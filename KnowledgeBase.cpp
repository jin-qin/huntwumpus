#include "KnowledgeBase.h"
#include "Tile.h"
#include "common.h"
#include "util.h"

using namespace std;

KnowledgeBase::KnowledgeBase(int row_len, int col_len)
             :m_row_len(row_len)
             ,m_col_len(col_len)
             ,m_counter(0)
{ 
    create_knowledge(row_len,col_len);   
}

KnowledgeBase::~KnowledgeBase(){
    safe_delete(m_pos_x);
    safe_delete(m_pos_y);
}
    
void KnowledgeBase::create_knowledge(int rows, int cols){ // Initialize the knowledge board
	m_map = util::create_new_map(rows, rows);

    safe_delete(m_pos_x);
    safe_delete(m_pos_y);

    //m_pos_x = new int[x*y];//To be initiated, may be -1?
    //m_pos_y = new int[x*y];//To be initiated
};

void KnowledgeBase::add_knowledge(int pos_x, int pos_y, Tile status){ // Receive the new information and add to current map
    //if there are pit, wumpus, gold, the game is over. Therefore in this case, the program will not call this func.
    //So we just need to think about breezy and smelly.
    if(status.is_entrance()){
        m_map[pos_x][pos_y]->add_state(Tile::TS_ENTRANCE);
    }
    if(status.is_breezy()){
        m_map[pos_x][pos_y]->add_state(Tile::TS_BREEZY);
        if(1 == m_col_len && 2 == m_row_len){
            if(0 == pos_x){
                m_map[pos_x+1][pos_y]->add_state(Tile::TS_PIT);
                m_map[pos_x+1][pos_y]->add_state(Tile::TS_DETERMINED);
            }else{
                m_map[pos_x-1][pos_y]->add_state(Tile::TS_PIT);
                m_map[pos_x-1][pos_y]->add_state(Tile::TS_DETERMINED);
            }
        }else if(2 == m_col_len && 1 == m_row_len){
            if(0 == pos_y){
                m_map[pos_x][pos_y+1]->add_state(Tile::TS_PIT);
                m_map[pos_x][pos_y+1]->add_state(Tile::TS_DETERMINED);
            }else{
                m_map[pos_x][pos_y-1]->add_state(Tile::TS_PIT);
                m_map[pos_x][pos_y-1]->add_state(Tile::TS_DETERMINED);
            }
        }else{
            if(pos_x > 0){
                m_map[pos_x-1][pos_y]->add_state(Tile::TS_PIT);
            }
            if(pos_x < m_col_len -1){
                m_map[pos_x+1][pos_y]->add_state(Tile::TS_PIT);
            }
            if(pos_y > 0){
                m_map[pos_x][pos_y-1]->add_state(Tile::TS_PIT);
            }
            if(pos_y < m_row_len -1){
                m_map[pos_y][pos_y+1]->add_state(Tile::TS_PIT);
            }
        }
    }
    if(status.is_smelly()){
        m_map[pos_x][pos_y]->add_state(Tile::TS_SMELLY);
        if(1 == m_col_len && 2 == m_row_len){
            if(0 == pos_x){
                m_map[pos_x+1][pos_y]->add_state(Tile::TS_WUMPUS);
                m_map[pos_x+1][pos_y]->add_state(Tile::TS_DETERMINED);
            }else{
                m_map[pos_x-1][pos_y]->add_state(Tile::TS_WUMPUS);
                m_map[pos_x-1][pos_y]->add_state(Tile::TS_DETERMINED);
            }
        }else if(2 == m_col_len && 1 == m_row_len){
            if(0 == pos_y){
                m_map[pos_x][pos_y+1]->add_state(Tile::TS_WUMPUS);
                m_map[pos_x][pos_y+1]->add_state(Tile::TS_DETERMINED);
            }else{
                m_map[pos_x][pos_y-1]->add_state(Tile::TS_WUMPUS);
                m_map[pos_x][pos_y-1]->add_state(Tile::TS_DETERMINED);
            }
        }else{
            if(pos_x > 0){
                m_map[pos_x-1][pos_y]->add_state(Tile::TS_WUMPUS);
            }
            if(pos_x < m_col_len -1){
                m_map[pos_x+1][pos_y]->add_state(Tile::TS_WUMPUS);
            }
            if(pos_y > 0){
                m_map[pos_x][pos_y-1]->add_state(Tile::TS_WUMPUS);
            }
            if(pos_y < m_row_len -1){
                m_map[pos_y][pos_y+1]->add_state(Tile::TS_WUMPUS);
            }
            // if(0 == pos_x){
            //    if(0 <= pos_y - 1 && pos_y + 1 < m_col_len){
            //        m_map[pos_x+1][pos_y]->add_state(Tile::TS_WUMPUS);
            //        m_map[pos_x][pos_y-1]->add_state(Tile::TS_WUMPUS);
            //        m_map[pos_x][pos_y+1]->add_state(Tile::TS_WUMPUS);
            //    }else if(0 == pos_y){
            //        m_map[pos_x+1][pos_y]->add_state(Tile::TS_WUMPUS);
            //        m_map[pos_x][pos_y+1]->add_state(Tile::TS_WUMPUS);
            //    }else if(m_col_len == pos_y){
            //        m_map[pos_x+1][pos_y]->add_state(Tile::TS_WUMPUS);
            //        m_map[pos_x][pos_y-1]->add_state(Tile::TS_WUMPUS);
            //    }
            // }else if(m_col_len == pos_x){
            //     if(0 <= pos_y - 1 && pos_y + 1 < m_col_len){
            //        m_map[pos_x-1][pos_y]->add_state(Tile::TS_WUMPUS);
            //        m_map[pos_x][pos_y-1]->add_state(Tile::TS_WUMPUS);
            //        m_map[pos_x][pos_y+1]->add_state(Tile::TS_WUMPUS);
            //    }else if(0 == pos_y){
            //        m_map[pos_x-1][pos_y]->add_state(Tile::TS_WUMPUS);
            //        m_map[pos_x][pos_y+1]->add_state(Tile::TS_WUMPUS);
            //    }else if(m_col_len == pos_y){
            //        m_map[pos_x-1][pos_y]->add_state(Tile::TS_WUMPUS);
            //        m_map[pos_x][pos_y-1]->add_state(Tile::TS_WUMPUS);
            //    }
            // }else{
            //     if(0 <= pos_y - 1 && pos_y + 1 < m_col_len){
            //        m_map[pos_x+1][pos_y]->add_state(Tile::TS_WUMPUS);
            //        m_map[pos_x-1][pos_y]->add_state(Tile::TS_WUMPUS);
            //        m_map[pos_x][pos_y-1]->add_state(Tile::TS_WUMPUS);
            //        m_map[pos_x][pos_y+1]->add_state(Tile::TS_WUMPUS);
            //    }else if(0 == pos_y){
            //        m_map[pos_x+1][pos_y]->add_state(Tile::TS_WUMPUS);
            //        m_map[pos_x-1][pos_y]->add_state(Tile::TS_WUMPUS);
            //        m_map[pos_x][pos_y+1]->add_state(Tile::TS_WUMPUS);
            //    }else if(m_col_len == pos_y){
            //        m_map[pos_x+1][pos_y]->add_state(Tile::TS_WUMPUS);
            //        m_map[pos_x-1][pos_y]->add_state(Tile::TS_WUMPUS);
            //        m_map[pos_x][pos_y-1]->add_state(Tile::TS_WUMPUS);
            //    }
            // }
        }
    }
    m_pos_x[m_counter] = pos_x;
    m_pos_y[m_counter] = pos_y;
    m_counter++;
};

void KnowledgeBase::analyze(int cur_x, int cur_y){//use the current information to infer the known space
    //rule: if 3 of 4 points around the position which is smelly or breezy is determined, the remaining point is the pit/wumpus.
    //search rule: searching from current position to the first position by using array m_pos_x and m_pos_y.
};

void KnowledgeBase::update_knowledge(){

}





