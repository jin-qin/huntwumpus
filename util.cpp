#include "util.h"
#include "Tile.h"
#include "Board.h"
#include <iostream>

namespace util {

void display_board(const Board::Map &map) {   
    for (size_t i = 0; i < map.size(); i++) {
        for (size_t j = 0; j < map[i].size(); j++) {
            int empty = 1;
            std::cout << "  " << i << "," << j << ": ";
            if (map[i][j]->is_entrance()) {
                empty = 0;
                std::cout << "entrance";
            }
            if (map[i][j]->is_breezy()) {
                empty = 0;
                std::cout << "breezy";
            }
            if (map[i][j]->is_smelly()) {
                empty = 0;
                std::cout << "smelly";
            }
            if (map[i][j]->has_wumpus()) {
                empty = 0;
                std::cout << "wumpus";
            }
            if (map[i][j]->has_gold()) {
                empty = 0;
                std::cout << "gold";
            }
            if (map[i][j]->has_pit()) {
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

Board::Map create_new_map(int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        std::cout << __FUNCTION__ << "::wrong rows or cols specified!" << std::endl;
        return Board::Map();
    }

    Board::Map map(rows, std::vector<Tile*>(cols, new Tile()));

    if (!map[0][0]) {
        std::cout << __FUNCTION__ << "::fatal error, empty pointer of the map, this should not appear!" << std::endl;
        return Board::Map();
    }
    map[0][0]->set_state(Tile::TS_ENTRANCE);

    return map;
}

void destroy_map(Board::Map &map) {
    for (size_t i = 0; i < map.size(); i++) {
        for (size_t j = 0; j < map[i].size(); j++) {
            safe_delete(map[i][j]);
        }
    }
    
    map.clear();
}

}