/**
 * @author Jing Qin <qinjin@bu.edu>
 */

#ifndef UTIL_H
#define UTIL_H

#include "Board.h"

namespace util {

void display_board(const Board::Map &map);

Board::Map create_new_map(int rows, int cols);
void destroy_map(Board::Map &map);

}

#endif // UTIL_H