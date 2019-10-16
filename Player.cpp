//
//  Player.cpp
//  
//
//  Created by Jonathan Redwine on 10/9/19.
//

#include "Player.h"
#include <iostream>
using namespace std;

void Player::move(int thisMove) {
    if (thisMove == 0) { // moving north
        currPosY += -1;
    }
    if (thisMove == 1) { // moving east
        currPosX += 1;
    }
    if (thisMove == 2) { // moving south
        currPosY += 1;
    }
    if (thisMove == 3) { // moving west
        currPosX += -1;
    }
}

int Player::select_move() {
    int thisMove = -1;
    int possibleMoves[4] = {-1, -1, -1, -1}; // given currPos, possible board movements
    if (currPosX == 0) { // if on the left edge, can only move right in x-direction
        possibleMoves[1] = 1;
    } else if (currPosX == 4) { // if on right edge, can only move left
        possibleMoves[3] = 3;
    } else {
        possibleMoves[1] = 1; // if in middle, can move both left and right
        possibleMoves[3] = 3;
    }
    if (currPosY == 0) { // if on top edge, can only move down
        possibleMoves[2] = 2;
    } else if (currPosY == 4) { // if on bottom edge, can only move up
        possibleMoves[0] = 0;
    } else {
        possibleMoves[2] = 2; // if in middle, can move up or down
        possibleMoves[0] = 0;
    }
    
    // if there are no adjacent dangers
    if (!knownBoard[currPosX][currPosY].is_breezy() && !knownBoard[currPosX][currPosY].is_smelly()) {
        thisMove = find_tile_not_yet_visited(possibleMoves);
    }
    
    move(thisMove);
    return thisMove;
}

int Player::find_tile_not_yet_visited(int possibleMoves[4]) {
    if (possibleMoves[0] != -1) { // check if north tile has been visited
        if (!knownBoard[currPosX][currPosY-1].player_been_here()) {
            return possibleMoves[0];
        }
    }
    if (possibleMoves[1] != -1) { // check if east tile has been visited
        if (!knownBoard[currPosX+1][currPosY].player_been_here()) {
            return possibleMoves[1];
        }
    }
    if (possibleMoves[2] != -1) { // check if south tile has been visited
        if (!knownBoard[currPosX][currPosY+1].player_been_here()) {
            return possibleMoves[2];
        }
    }
    if (possibleMoves[3] != -1) { // check is west tile has been visited
        if (!knownBoard[currPosX-1][currPosY].player_been_here()) {
            return possibleMoves[3];
        }
    }
    for (int i = 0; i < 4; i++) {
        if (possibleMoves[i] != -1) {
            return possibleMoves[i];
        }
    }
}
