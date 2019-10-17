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
}
    
void KnowledgeBase::create_knowledge(int x, int y){ // Initialize the knowledge board
    safe_delete(m_map);

    m_map = new Tile[x][y]();
    for(int i = 0; i < x; i++)
        for(int j = 0; j < y; j++){
            m_map[i][j] = Tile(Tile::TS_UNKNOWN);
        }
};

void KnowledgeBase::add_knowledge(int pos_x, int pos_y, Tile::TileState status){ // Receive the new information and add to current map
    //if there are pit, wumpus, gold, the game is over. Therefore in this case, the program will not call this func.
    //So we just need to think about breezy and smelly.
    map[pos_x][pos_y].isSafe = 1;
    if(1 == status[pos_x][pos_y].isBreezy){
        if(0 <= 1 - pos_x && x_len >= 1 + pos_x){
            if(0 <= 1 - pos_y && y_len >= 1 + pos_y){
                map[pos_x-1][pos_y].hasPit == 2;
                map[pos_x+1][pos_y].hasPit == 2;
                map[pos_x][pos_y-1].hasPit == 2;
                map[pos_x][pos_y+1].hasPit == 2;
            }
            else if(0 > 1 - pos_y){
                map[pos_x-1][pos_y].hasPit == 2;
                map[pos_x+1][pos_y].hasPit == 2;
                map[pos_x][pos_y+1].hasPit == 2;
            }else{
                map[pos_x-1][pos_y].hasPit == 2;
                map[pos_x+1][pos_y].hasPit == 2;
                map[pos_x][pos_y-1].hasPit == 2;
            }
        }else if(0 > 1 - pos_x){
            if(0 <= 1 - pos_y && y_len >= 1 + pos_y){
                map[pos_x+1][pos_y].hasPit == 2;
                map[pos_x][pos_y-1].hasPit == 2;
                map[pos_x][pos_y+1].hasPit == 2;
            }
            else if(0 > 1 - pos_y){
                map[pos_x+1][pos_y].hasPit == 2;
                map[pos_x][pos_y+1].hasPit == 2;
            }else{
                map[pos_x+1][pos_y].hasPit == 2;
                map[pos_x][pos_y-1].hasPit == 2;
            }
        }else{
            if(0 <= 1 - pos_y && y_len >= 1 + pos_y){
                map[pos_x-1][pos_y].hasPit == 2;
                map[pos_x][pos_y-1].hasPit == 2;
                map[pos_x][pos_y+1].hasPit == 2;
            }
            else if(0 > 1 - pos_y){
                map[pos_x-1][pos_y].hasPit == 2;
                map[pos_x][pos_y+1].hasPit == 2;
            }else{
                map[pos_x-1][pos_y].hasPit == 2;
                map[pos_x][pos_y-1].hasPit == 2;
            }
        }
    }
    if(1 == status[pos_x][pos_y].isSmelly){
        if(0 <= 1 - pos_x && x_len >= 1 + pos_x){
            if(0 <= 1 - pos_y && y_len >= 1 + pos_y){
                map[pos_x-1][pos_y].hasWumpus == 2;
                map[pos_x+1][pos_y].hasWumpus == 2;
                map[pos_x][pos_y-1].hasWumpus == 2;
                map[pos_x][pos_y+1].hasWumpus == 2;
            }
            else if(0 > 1 - pos_y){
                map[pos_x-1][pos_y].hasWumpus == 2;
                map[pos_x+1][pos_y].hasWumpus == 2;
                map[pos_x][pos_y+1].hasWumpus == 2;
            }else{
                map[pos_x-1][pos_y].hasWumpus == 2;
                map[pos_x+1][pos_y].hasWumpus == 2;
                map[pos_x][pos_y-1].hasWumpus == 2;
            }
        }else if(0 > 1 - pos_x){
            if(0 <= 1 - pos_y && y_len >= 1 + pos_y){
                map[pos_x+1][pos_y].hasWumpus == 2;
                map[pos_x][pos_y-1].hasWumpus == 2;
                map[pos_x][pos_y+1].hasWumpus == 2;
            }
            else if(0 > 1 - pos_y){
                map[pos_x+1][pos_y].hasWumpus == 2;
                map[pos_x][pos_y+1].hasWumpus == 2;
            }else{
                map[pos_x+1][pos_y].hasWumpus == 2;
                map[pos_x][pos_y-1].hasWumpus == 2;
            }
        }else{
            if(0 <= 1 - pos_y && y_len >= 1 + pos_y){
                map[pos_x-1][pos_y].hasWumpus == 2;
                map[pos_x][pos_y-1].hasWumpus == 2;
                map[pos_x][pos_y+1].hasWumpus == 2;
            }
            else if(0 > 1 - pos_y){
                map[pos_x-1][pos_y].hasWumpus == 2;
                map[pos_x][pos_y+1].hasWumpus == 2;
            }else{
                map[pos_x-1][pos_y].hasWumpus == 2;
                map[pos_x][pos_y-1].hasWumpus == 2;
            }
        }
    }
};

void KnowledgeBasee::analyze(int cur_x, int cur_y){//use the current information to infer the known space
    
};

void KnowledgeBase::update_knowledge(){

}





