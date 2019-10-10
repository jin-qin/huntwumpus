//
//  Player.cpp
//  
//
//  Created by Jonathan Redwine on 10/9/19.
//

#include <stdio.h>
using namespace std;


class Player {
public:
    int currPosX;
    int currPosY;
    Tile knownBoard[5][5] =
    {{Tile(1,0,0,0,0,0,1,1), //0,0
        Tile(0,2,2,0,2,0,0,0), //0,1
        Tile(0,2,2,2,2,2,0,0), //0,2
        Tile(0,2,2,2,2,2,0,0), //0,3
        Tile(0,2,2,2,2,2,0,0)}, //0,4
        {Tile(0,2,2,0,2,0,0,0), //1,0
            Tile(0,2,2,2,2,2,0,0), //1,1
            Tile(0,2,2,2,2,2,0,0), //1,2
            Tile(0,2,2,2,2,2,0,0), //1,3
            Tile(0,2,2,2,2,2,0,0)}, //1,4
        {Tile(0,2,2,2,2,2,0,0), //2,0
            Tile(0,2,2,2,2,2,0,0), //2,1
            Tile(0,2,2,2,2,2,0,0), //2,2
            Tile(0,2,2,2,2,2,0,0), //2,3
            Tile(0,2,2,2,2,2,0,0)}, //2,4
        {Tile(0,2,2,2,2,2,0,0), //3,0
            Tile(0,2,2,2,2,2,0,0), //3,1
            Tile(0,2,2,2,2,2,0,0), //3,2
            Tile(0,2,2,2,2,2,0,0), //3,3
            Tile(0,2,2,2,2,2,0,0)}, //3,4
        {Tile(0,2,2,2,2,2,0,0), //4,0
            Tile(0,2,2,2,2,2,0,0), //4,1
            Tile(0,2,2,2,2,2,0,0), //4,2
            Tile(0,2,2,2,2,2,0,0), //4,3
            Tile(0,2,2,2,2,2,0,0)} //4,4
    };

    Player() {
        currPosX = 0;
        currPosY = 0;
    }
    
    void move(int thisMove);
    int selectMove();
    int findTileNotYetVisited(int possibleMoves[4]);
};


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

int Player::selectMove() {
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
    if (knownBoard[currPosX][currPosY].isBreezy == 0 && knownBoard[currPosX][currPosY].isSmelly == 0) {
        thisMove = findTileNotYetVisited(possibleMoves);
    }
    
    move(thisMove);
    return thisMove;
}

int Player::findTileNotYetVisited(int possibleMoves[4]) {
    if (possibleMoves[0] != -1) { // check if north tile has been visited
        if (knownBoard[currPosX][currPosY-1].playerBeenHere == 0) {
            return possibleMoves[0];
        }
    }
    if (possibleMoves[1] != -1) { // check if east tile has been visited
        if (knownBoard[currPosX+1][currPosY].playerBeenHere == 0) {
            return possibleMoves[1];
        }
    }
    if (possibleMoves[2] != -1) { // check if south tile has been visited
        if (knownBoard[currPosX][currPosY+1].playerBeenHere == 0) {
            return possibleMoves[2];
        }
    }
    if (possibleMoves[3] != -1) { // check is west tile has been visited
        if (knownBoard[currPosX-1][currPosY].playerBeenHere == 0) {
            return possibleMoves[3];
        }
    }
    for (int i = 0; i < 4; i++) {
        if (possibleMoves[i] != -1) {
            return possibleMoves[i];
        }
    }
}
