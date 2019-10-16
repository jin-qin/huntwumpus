//
//  Player.h
//  
//
//  Created by Jonathan Redwine on 10/9/19.
//

#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include "Tile.h"

class Player {
public:
    int currPosX;
    int currPosY;
    Tile **knownBoard = 0; // TODO

    // Tile knownBoard[5][5] =
    // {{Tile(Tile::TS_ENTRANCE | Tile::TS_PLAYER_HERE | Tile::TS_DETERMINED), //0,0
    //     Tile(0,2,2,0,2,0,0,0), //0,1
    //     Tile(0,2,2,2,2,2,0,0), //0,2
    //     Tile(0,2,2,2,2,2,0,0), //0,3
    //     Tile(0,2,2,2,2,2,0,0)}, //0,4
    //     {Tile(0,2,2,0,2,0,0,0), //1,0
    //         Tile(0,2,2,2,2,2,0,0), //1,1
    //         Tile(0,2,2,2,2,2,0,0), //1,2
    //         Tile(0,2,2,2,2,2,0,0), //1,3
    //         Tile(0,2,2,2,2,2,0,0)}, //1,4
    //     {Tile(0,2,2,2,2,2,0,0), //2,0
    //         Tile(0,2,2,2,2,2,0,0), //2,1
    //         Tile(0,2,2,2,2,2,0,0), //2,2
    //         Tile(0,2,2,2,2,2,0,0), //2,3
    //         Tile(0,2,2,2,2,2,0,0)}, //2,4
    //     {Tile(0,2,2,2,2,2,0,0), //3,0
    //         Tile(0,2,2,2,2,2,0,0), //3,1
    //         Tile(0,2,2,2,2,2,0,0), //3,2
    //         Tile(0,2,2,2,2,2,0,0), //3,3
    //         Tile(0,2,2,2,2,2,0,0)}, //3,4
    //     {Tile(0,2,2,2,2,2,0,0), //4,0
    //         Tile(0,2,2,2,2,2,0,0), //4,1
    //         Tile(0,2,2,2,2,2,0,0), //4,2
    //         Tile(0,2,2,2,2,2,0,0), //4,3
    //         Tile(0,2,2,2,2,2,0,0)} //4,4
    // };

    Player() {
        currPosX = 0;
        currPosY = 0;
    }
    
    void move(int thisMove);
    int select_move();
    int find_tile_not_yet_visited(int possibleMoves[4]);

private:
    int m_rows = 0;
    int m_cols = 0;
};

#endif //PLAYER_H