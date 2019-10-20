/**
 * @author Jing Qin <qinjin@bu.edu>
 */

#ifndef BOARD_H
#define BOARD_H

#include "common.h"
#include <vector>

class Tile;
class Board {
public:
    typedef std::vector< std::vector<Tile*> > Map;

public:
    Board();
    ~Board();

    void display_board();

    void generate_new_map(int rows, int cols);

    inline int rows() {return m_rows;}
    inline int cols() {return m_cols;}

    bool try_kill_wumpus(int row, int col, MoveDirection md);

private:
    void generate_breezy_of_pit(int row, int col);
    void generate_smelly_of_wumpus(int row, int col);

    void cleanup();

    void clear_smelly_and_wumpus();

private:
    int m_rows = 0;
    int m_cols = 0;

    Map m_map;

    int m_row_wumpus = -1;
    int m_col_wumpus = -1;
    int m_row_gold = -1;
    int m_col_gold = -1;
};

#endif // BOARD_H