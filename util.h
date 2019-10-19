/**
 * @author Jing Qin <qinjin@bu.edu>
 */

#ifndef UTIL_H
#define UTIL_H

#include "Board.h"

namespace util {

void display_board(const Board::Map &map);

/**
 * @brief a new map is an empty map which (0,0) is the entrance. 
 */ 
Board::Map create_new_map(int rows, int cols);
void destroy_map(Board::Map &map);

}

#endif // UTIL_H