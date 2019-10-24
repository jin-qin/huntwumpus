#include "KnowledgeBase.h"
#include "Tile.h"
#include "common.h"
#include "util.h"
#include <utility>

using namespace std;

KnowledgeBase::KnowledgeBase(int row_len, int col_len)
             :m_row_len(row_len)
             ,m_col_len(col_len)
{ 
    create_knowledge(row_len,col_len);   
}

KnowledgeBase::~KnowledgeBase(){

}

void KnowledgeBase::judge_status(){
    
}
  
void KnowledgeBase::create_knowledge(int rows, int cols){ // Initialize the knowledge board
	m_map = util::create_new_map(rows, rows);

    //m_row = new int[x*y];//To be initiated, may be -1?
    //m_pos_y = new int[x*y];//To be initiated
}

void KnowledgeBase::add_knowledge(int row, int col, Tile status){ // Receive the new information and add to current map
    //if there are pit, wumpus, gold, the game is over. Therefore in this case, the program will not call this func.
    //So we just need to think about breezy and smelly.
    if(status.is_entrance()){
        m_map[row][col]->add_state(Tile::TS_ENTRANCE);
    }

    using namespace std;
    vector< pair<Tile::TileState, Tile::TileState> > target_states;

    if (status.is_breezy()) target_states.push_back(make_pair(Tile::TS_BREEZY, Tile::TS_PIT));
    if (status.is_smelly()) target_states.push_back(make_pair(Tile::TS_SMELLY, Tile::TS_WUMPUS));
    
    for (int i = 0; i < target_states.size(); i++) {
        m_map[row][col]->add_state(target_states[i].first);
        if(row > 0)
            m_map[row][col]->add_state(target_states[i].second);
        if(row < m_row_len -1)
            m_map[row+1][col]->add_state(target_states[i].second);
        if(col > 0)
            m_map[row][col-1]->add_state(target_states[i].second);
        if(col < m_col_len -1)
            m_map[row][col+1]->add_state(target_states[i].second);
    }

    add_knowledge_into_history(row, col);
    //To continue
};

void KnowledgeBase::add_knowledge_into_history(int row, int col) {
    for (int i = 0; i < m_history_pos.size(); i++) {
        if (m_history_pos[i].m_row == row && m_history_pos[i].m_col == col)
            return;
    }

    m_history_pos.push_back(Position(row, col));
}

void KnowledgeBase::analyze(int cur_row, int cur_col){//use the current information to infer the known space
    //rule: if 3 of 4 points around the position which is smelly or breezy is safe, the remaining point is the pit/wumpus.
    //search rule: searching from current position to the first position by using array m_row and m_pos_y.
    int row, col;
    row = cur_row;
    col = cur_col;
    //TO DO
    while(true){//Q:set_state or add_state?
        if(!m_map[row][col]->is_breezy()&&!m_map[row][col]->is_smelly()){
            m_map[row][col]->set_state(Tile::TS_SAFE);
            if(row > 0){
                m_map[row-1][col]->set_state(Tile::TS_SAFE);
            }
            if(row < m_col_len -1){
                m_map[row+1][col]->set_state(Tile::TS_SAFE);
            }
            if(col > 0){
                m_map[row][col-1]->set_state(Tile::TS_SAFE);
            }
            if(col < m_row_len -1){
                m_map[col][col+1]->set_state(Tile::TS_SAFE);
            }
        }

        if(m_map[row][col]->is_breezy()){
            if(0 == row){
                if(0 == col){
                    if(m_map[row+1][col]->is_safe()){
                        m_map[row][col+1]->add_state(Tile::TS_PIT);
                        m_map[row][col+1]->add_state(Tile::TS_DETERMINED);
                    }
                    if(m_map[row][col+1]->is_safe()){
                        m_map[row+1][col]->add_state(Tile::TS_PIT);
                        m_map[row+1][col]->add_state(Tile::TS_DETERMINED);
                    }
                }
                else if(m_col_len-1 == col){
                    if(m_map[row][col-1]->is_safe()){
                        m_map[row+1][col]->add_state(Tile::TS_PIT);
                        m_map[row+1][col]->add_state(Tile::TS_DETERMINED);
                    }
                    if(m_map[row+1][col]->is_safe()){
                        m_map[row][col-1]->add_state(Tile::TS_PIT);
                        m_map[row][col-1]->add_state(Tile::TS_PIT);
                    }
                }
                else{
                    if(!(m_map[row][col-1]->is_safe()^m_map[row][col+1]->is_safe()^m_map[row+1][col]->is_safe())){
                        if(!m_map[row][col-1]->is_safe()){
                            m_map[row][col-1]->add_state(Tile::TS_PIT);
                            m_map[row][col-1]->add_state(Tile::TS_DETERMINED);
                        }
                        if(!m_map[row][col+1]->is_safe()){
                            m_map[row][col+1]->add_state(Tile::TS_PIT);
                            m_map[row][col+1]->add_state(Tile::TS_DETERMINED);
                        }
                        if(!m_map[row+1][col]->is_safe()){
                            m_map[row+1][col]->add_state(Tile::TS_PIT);
                            m_map[row+1][col]->add_state(Tile::TS_DETERMINED);
                        }
                    }
                }
            }else if(m_row_len - 1 == row){
                if(0 == col){
                    if(m_map[row-1][col]->is_safe()){
                        m_map[row][col+1]->add_state(Tile::TS_PIT);
                        m_map[row][col+1]->add_state(Tile::TS_DETERMINED);
                    }
                    if(m_map[row][col+1]->is_safe()){
                        m_map[row-1][col]->add_state(Tile::TS_PIT);
                        m_map[row-1][col]->add_state(Tile::TS_DETERMINED);
                    }
                }
                else if(m_col_len-1 == col){
                    if(m_map[row][col-1]->is_safe()){
                        m_map[row-1][col]->add_state(Tile::TS_PIT);
                        m_map[row-1][col]->add_state(Tile::TS_DETERMINED);
                    }
                    if(m_map[row-1][col]->is_safe()){
                        m_map[row][col-1]->add_state(Tile::TS_PIT);
                        m_map[row][col-1]->add_state(Tile::TS_PIT);
                    }
                }
                else{
                    bool p,q;
                    p = false;
                    q = false;
                    p = !(m_map[row][col-1]->is_safe()^m_map[row][col+1]->is_safe()^m_map[row-1][col]->is_safe());
                    q = !(m_map[row][col-1]->is_safe()||m_map[row][col+1]->is_safe()||m_map[row-1][col]->is_safe());
                    if(p && q){//need modification
                        if(!m_map[row][col-1]->is_safe()){
                            m_map[row][col-1]->add_state(Tile::TS_PIT);
                            m_map[row][col-1]->add_state(Tile::TS_DETERMINED);
                        }
                        if(!m_map[row][col+1]->is_safe()){
                            m_map[row][col+1]->add_state(Tile::TS_PIT);
                            m_map[row][col+1]->add_state(Tile::TS_DETERMINED);
                        }
                        if(!m_map[row-1][col]->is_safe()){
                            m_map[row-1][col]->add_state(Tile::TS_PIT);
                            m_map[row-1][col]->add_state(Tile::TS_DETERMINED);
                        }
                    }
                }
            }else{
                if(0 == col){

                }else if(m_col_len - 1 == row){

                }else{

                }
            }
        }

        if(m_map[row][col]->is_smelly()){

        }
    }
};

void KnowledgeBase::update_knowledge(){

}





