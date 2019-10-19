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

private:
    void generate_breezy_of_pit(int row, int col);
    void generate_smelly_of_wumpus(int row, int col);

private:
    int m_rows = 0;
    int m_cols = 0;

    Map m_map;
};

#endif // BOARD_H