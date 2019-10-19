/**
 * @author Jonathan Redwine
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include "Tile.h"

class Player {
public:
    int currPosX;
    int currPosY;
    Tile **knownBoard = 0; // TODO

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