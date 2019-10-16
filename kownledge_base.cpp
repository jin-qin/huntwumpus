#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<Tile.cpp>

using namespace std;

class Knowledge {
public:
    int isEntrance;// Is this var necessary?
    int isBreezy;
    int isSmelly;
    int hasWumpus;
    int hasGold;
    int hasPit;
    int hasNothing;
    int playerBeenHere;
    int isInferred;
    int isSafe;
    Knowledge(int e, int b, int s, int w, int g, int p, int n, int pbh, int i, int s) {
        isEntrance = e;
        isBreezy = b;
        isSmelly = s;
        hasWumpus = w;
        hasGold = g;
        hasPit = p;
        hasNothing = n;
        playerBeenHere = pbh;
        isInferred = i;
        isSafe = s;
    }
};

class Knowledge_base{
public:
    void add_knowledge(int pos_x, int pos_y, int status);
    void create_knowledge(int x, int y);
    void analyze();
    void update_knowledge();
    int x_len, y_len;
};

void Knowledge_base::create_knowledge(int x, int y){ // Initialize the knowledge board
    Knowledge map[x][y];
    for(int i = 0; i < x; i++)
        for(int j = 0; j < y; j++){
            map[i][j] = Knowledge(2,2,2,2,2,2,2,0,0,2);
        }
};

void Knowledge_base::add_knowledge(int pos_x, int pos_y, Tile status){ // Receive the new information and add to current map
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

void Knowledge_base::analyze(int cur_x, int cur_y){//use the current information to infer the known space
    
};

void Knowledge_base::update_knowledge(){

}





