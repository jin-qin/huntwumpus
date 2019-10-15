//
//  Board.cpp
//  
//
//  Created by Jonathan Redwine on 10/9/19.
//

#include <stdio.h>
#include "Tile.cpp"
using namespace std;

class Board {
public:
    Tile array[5][5] =
    {{Tile(1,0,0,0,0,0,0,0), //0,0
        Tile(0,1,0,0,0,0,0,0), //0,1
        Tile(0,0,0,0,0,1,0,0), //0,2
        Tile(0,1,0,0,0,0,0,0), //0,3
        Tile(0,0,0,0,0,0,0,0)}, //0,4
        {Tile(0,0,0,0,0,0,0,0), //1,0
            Tile(0,0,0,0,0,0,0,0), //1,1
            Tile(0,1,0,0,0,0,0,0), //1,2
            Tile(0,0,0,0,0,0,0,0), //1,3
            Tile(0,0,0,0,0,0,0,0)}, //1,4
        {Tile(0,1,0,0,0,0,0,0), //2,0
            Tile(0,0,0,0,0,0,0,0), //2,1
            Tile(0,0,1,0,0,0,0,0), //2,2
            Tile(0,0,0,0,0,0,0,0), //2,3
            Tile(0,1,0,0,0,0,0,0)}, //2,4
        {Tile(0,0,0,0,0,1,0,0), //3,0
            Tile(0,1,1,0,0,0,0,0), //3,1
            Tile(0,0,0,1,0,0,0,0), //3,2
            Tile(0,1,1,0,1,0,0,0), //3,3
            Tile(0,0,0,0,0,1,0,0)}, //3,4
        {Tile(0,1,0,0,0,0,0,0), //4,0
            Tile(0,0,0,0,0,1,0,0), //4,1
            Tile(0,1,1,0,0,0,0,0), //4,2
            Tile(0,0,0,0,0,0,0,0), //4,3
            Tile(0,1,0,0,0,0,0,0)} //4,4
    };
    
    void displayBoard();
};

void Board::displayBoard() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            int empty = 1;
            cout << "  " << i << "," << j << ": ";
            if (array[i][j].isEntrance == 1) {
                empty = 0;
                cout << "entrance";
            }
            if (array[i][j].isBreezy == 1) {
                empty = 0;
                cout << "breezy";
            }
            if (array[i][j].isSmelly == 1) {
                empty = 0;
                cout << "smelly";
            }
            if (array[i][j].hasWumpus == 1) {
                empty = 0;
                cout << "wumpus";
            }
            if (array[i][j].hasGold == 1) {
                empty = 0;
                cout << "gold";
            }
            if (array[i][j].hasPit == 1) {
                empty = 0;
                cout << "pit";
            }
            if (empty) {
                cout << "empty";
            }
        }
        cout << endl;
    }
}
