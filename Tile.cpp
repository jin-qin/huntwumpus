//
//  Tile.cpp
//  
//
//  Created by Jonathan Redwine on 10/9/19.
//

#include <stdio.h>
using namespace std;


class Tile {
public:
    int isEntrance;
    int isBreezy;
    int isSmelly;
    int hasWumpus;
    int hasGold;
    int hasPit;
    int playerBeenHere;
    int determined;
    Tile(int e, int b, int s, int w, int g, int p, int pbh, int d) {
        isEntrance = e;
        isBreezy = b;
        isSmelly = s;
        hasWumpus = w;
        hasGold = g;
        hasPit = p;
        playerBeenHere = pbh;
        determined = d;
    }
};
