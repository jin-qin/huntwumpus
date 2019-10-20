#include "KnowledgeBase.h"
#include "Tile.h"
#include "common.h"

using namespace std;

KnowledgeBase::KnowledgeBase(int row_len, int col_len)
             :m_row_len(row_len)
             :m_col_len(col_len)
{ 
    create_knowledge(row_len,col_len);   
}

KnowledgeBase::~KnowledgeBase(void){
    safe_delete(m_map);
    safe_delete(m_pos_x);
    safe_delete(m_pos_y);
}
    
void KnowledgeBase::create_knowledge(int x, int y){ // Initialize the knowledge board
    safe_delete(m_map);
    safe_delete(m_pos_x);
    safe_delete(m_pos_y);

    m_pos_x = new int[x*y];//To be initiated, may be -1?
    m_pos_y = new int[x*y];//To be initiated
    m_map = new Tile[x][y]();
    for(int i = 0; i < x; i++)
        for(int j = 0; j < y; j++){
            m_map[i][j] = Tile(Tile::TS_UNKNOWN);
        }
};

void KnowledgeBase::add_knowledge(int pos_x, int pos_y, Tile status){ // Receive the new information and add to current map
    //if there are pit, wumpus, gold, the game is over. Therefore in this case, the program will not call this func.
    //So we just need to think about breezy and smelly.
    if(status.is_entrance()){
        m_map[pos_x][pos_y].add_state(Tile::TS_ENTRANCE);
    }
    if(status.is_breezy()){
        m_map[pos_x][pos_y].add_state(Tile::TS_BREEZY);
        if(1 == m_col_len && 2 == m_row_len){
            if(0 == pos_x){
                m_map[pos_x+1][pos_y].add_state(Tile::TS_PIT);
                m_map[pos_x+1][pos_y].add_state(Tile::TS_DETERMINED);
            }else{
                m_map[pos_x-1][pos_y].add_state(Tile::TS_PIT);
                m_map[pos_x-1][pos_y].add_state(Tile::TS_DETERMINED);
            }
        }else if(2 == m_col_len && 1 == m_row_len){
            if(0 == pos_y){
                m_map[pos_x][pos_y+1].add_state(Tile::TS_PIT);
                m_map[pos_x][pos_y+1].add_state(Tile::TS_DETERMINED);
            }else{
                m_map[pos_x][pos_y-1].add_state(Tile::TS_PIT);
                m_map[pos_x][pos_y-1].add_state(Tile::TS_DETERMINED);
            }
        }else{
            if(0 == pos_x){
               if(0 <= pos_y - 1 && pos_y + 1 < m_col_len){
                   m_map[pos_x+1][pos_y].add_state(Tile::TS_PIT);
                   m_map[pos_x][pos_y-1].add_state(Tile::TS_PIT);
                   m_map[pos_x][pos_y+1].add_state(Tile::TS_PIT);
               }else if(0 == pos_y){
                   m_map[pos_x+1][pos_y].add_state(Tile::TS_PIT);
                   m_map[pos_x][pos_y+1].add_state(Tile::TS_PIT);
               }else if(m_col_len == pos_y){
                   m_map[pos_x+1][pos_y].add_state(Tile::TS_PIT);
                   m_map[pos_x][pos_y-1].add_state(Tile::TS_PIT);
               }
            }else if(m_col_len == pos_x){
                if(0 <= pos_y - 1 && pos_y + 1 < m_col_len){
                   m_map[pos_x-1][pos_y].add_state(Tile::TS_PIT);
                   m_map[pos_x][pos_y-1].add_state(Tile::TS_PIT);
                   m_map[pos_x][pos_y+1].add_state(Tile::TS_PIT);
               }else if(0 == pos_y){
                   m_map[pos_x-1][pos_y].add_state(Tile::TS_PIT);
                   m_map[pos_x][pos_y+1].add_state(Tile::TS_PIT);
               }else if(m_col_len == pos_y){
                   m_map[pos_x-1][pos_y].add_state(Tile::TS_PIT);
                   m_map[pos_x][pos_y-1].add_state(Tile::TS_PIT);
               }
            }else{
                if(0 <= pos_y - 1 && pos_y + 1 < m_col_len){
                   m_map[pos_x+1][pos_y].add_state(Tile::TS_PIT);
                   m_map[pos_x-1][pos_y].add_state(Tile::TS_PIT);
                   m_map[pos_x][pos_y-1].add_state(Tile::TS_PIT);
                   m_map[pos_x][pos_y+1].add_state(Tile::TS_PIT);
               }else if(0 == pos_y){
                   m_map[pos_x+1][pos_y].add_state(Tile::TS_PIT);
                   m_map[pos_x-1][pos_y].add_state(Tile::TS_PIT);
                   m_map[pos_x][pos_y+1].add_state(Tile::TS_PIT);
               }else if(m_col_len == pos_y){
                   m_map[pos_x+1][pos_y].add_state(Tile::TS_PIT);
                   m_map[pos_x-1][pos_y].add_state(Tile::TS_PIT);
                   m_map[pos_x][pos_y-1].add_state(Tile::TS_PIT);
               }
            }
        }
    }
    if(status.is_smelly()){
        m_map[pos_x][pos_y].add_state(Tile::TS_SMELLY);
        if(1 == m_col_len && 2 == m_row_len){
            if(0 == pos_x){
                m_map[pos_x+1][pos_y].add_state(Tile::TS_WUMPUS);
                m_map[pos_x+1][pos_y].add_state(Tile::TS_DETERMINED);
            }else{
                m_map[pos_x-1][pos_y].add_state(Tile::TS_WUMPUS);
                m_map[pos_x-1][pos_y].add_state(Tile::TS_DETERMINED);
            }
        }else if(2 == m_col_len && 1 == m_row_len){
            if(0 == pos_y){
                m_map[pos_x][pos_y+1].add_state(Tile::TS_WUMPUS);
                m_map[pos_x][pos_y+1].add_state(Tile::TS_DETERMINED);
            }else{
                m_map[pos_x][pos_y-1].add_state(Tile::TS_WUMPUS);
                m_map[pos_x][pos_y-1].add_state(Tile::TS_DETERMINED);
            }
        }else{
            if(0 == pos_x){
               if(0 <= pos_y - 1 && pos_y + 1 < m_col_len){
                   m_map[pos_x+1][pos_y].add_state(Tile::TS_WUMPUS);
                   m_map[pos_x][pos_y-1].add_state(Tile::TS_WUMPUS);
                   m_map[pos_x][pos_y+1].add_state(Tile::TS_WUMPUS);
               }else if(0 == pos_y){
                   m_map[pos_x+1][pos_y].add_state(Tile::TS_WUMPUS);
                   m_map[pos_x][pos_y+1].add_state(Tile::TS_WUMPUS);
               }else if(m_col_len == pos_y){
                   m_map[pos_x+1][pos_y].add_state(Tile::TS_WUMPUS);
                   m_map[pos_x][pos_y-1].add_state(Tile::TS_WUMPUS);
               }
            }else if(m_col_len == pos_x){
                if(0 <= pos_y - 1 && pos_y + 1 < m_col_len){
                   m_map[pos_x-1][pos_y].add_state(Tile::TS_WUMPUS);
                   m_map[pos_x][pos_y-1].add_state(Tile::TS_WUMPUS);
                   m_map[pos_x][pos_y+1].add_state(Tile::TS_WUMPUS);
               }else if(0 == pos_y){
                   m_map[pos_x-1][pos_y].add_state(Tile::TS_WUMPUS);
                   m_map[pos_x][pos_y+1].add_state(Tile::TS_WUMPUS);
               }else if(m_col_len == pos_y){
                   m_map[pos_x-1][pos_y].add_state(Tile::TS_WUMPUS);
                   m_map[pos_x][pos_y-1].add_state(Tile::TS_WUMPUS);
               }
            }else{
                if(0 <= pos_y - 1 && pos_y + 1 < m_col_len){
                   m_map[pos_x+1][pos_y].add_state(Tile::TS_WUMPUS);
                   m_map[pos_x-1][pos_y].add_state(Tile::TS_WUMPUS);
                   m_map[pos_x][pos_y-1].add_state(Tile::TS_WUMPUS);
                   m_map[pos_x][pos_y+1].add_state(Tile::TS_WUMPUS);
               }else if(0 == pos_y){
                   m_map[pos_x+1][pos_y].add_state(Tile::TS_WUMPUS);
                   m_map[pos_x-1][pos_y].add_state(Tile::TS_WUMPUS);
                   m_map[pos_x][pos_y+1].add_state(Tile::TS_WUMPUS);
               }else if(m_col_len == pos_y){
                   m_map[pos_x+1][pos_y].add_state(Tile::TS_WUMPUS);
                   m_map[pos_x-1][pos_y].add_state(Tile::TS_WUMPUS);
                   m_map[pos_x][pos_y-1].add_state(Tile::TS_WUMPUS);
               }
            }
        }
    }
};

void KnowledgeBasee::analyze(int cur_x, int cur_y){//use the current information to infer the known space
    //rule: if 3 of 4 points around the position which is smelly or breezy is determined, the remaining point is the pit/wumpus.
    //search rule: searching from current position to the first position by using array m_pos_x and m_pos_y.
};

void KnowledgeBase::update_knowledge(){

}





