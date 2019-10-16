/**
 * @author Jing Qin <qinjin@bu.edu>
 */

#ifndef BOARD_H
#define BOARD_H

#include "common.h"
#include "Tile.h"

class Board {
public:
    Board();

    void display_board();

    void generate_new_board(int rows, int cols);

    inline int rows() {return m_rows;}
    inline int cols() {return m_cols;}

private:
    int m_rows = 0;
    int m_cols = 0;

    Tile **m_map = 0;
};

#endif // BOARD_H