#include "util.h"
#include "Tile.h"
#include <iostream>

namespace util {

void display_board(Tile **array, int rows, int cols)
{
    if (!array) return;
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int empty = 1;
            std::cout << "  " << i << "," << j << ": ";
            if (array[i][j].is_entrance()) {
                empty = 0;
                std::cout << "entrance";
            }
            if (array[i][j].is_breezy()) {
                empty = 0;
                std::cout << "breezy";
            }
            if (array[i][j].is_smelly()) {
                empty = 0;
                std::cout << "smelly";
            }
            if (array[i][j].has_wumpus()) {
                empty = 0;
                std::cout << "wumpus";
            }
            if (array[i][j].has_gold()) {
                empty = 0;
                std::cout << "gold";
            }
            if (array[i][j].has_pit()) {
                empty = 0;
                std::cout << "pit";
            }
            if (empty) {
                std::cout << "empty";
            }
        }
        std::cout << std::endl;
    }
}

}