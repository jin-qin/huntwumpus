/**
 * @author Jing Qin <qinjin@bu.edu>
 */

#ifndef BOARD_H
#define BOARD_H

#include "common.h"
#include <vector>
#include <memory>

class Tile;
class Board {
public:
    typedef std::vector< std::vector< std::shared_ptr<Tile> > > Map;

public:
    Board();
    ~Board();

    void display_board();

    PositionList neighbors(const Position &pos);
    
    std::shared_ptr<Tile> tile(const Position &pos);

    void generate_new_map(int rows, int cols);
    void set_map(const Map &map);

    inline int rows() {return m_rows;}
    inline int cols() {return m_cols;}

    bool try_kill_wumpus(const Position &pos, MoveDirection md);

private:
    void generate_breezy_of_pit(const Position &pos);
    void generate_smelly_of_wumpus(const Position &pos);

    void cleanup();

    void clear_smelly_and_wumpus();

private:
    Map m_map;
    
    int m_rows = 0;
    int m_cols = 0;

    Position m_pos_wumpus = Position(-1, -1);
    Position m_pos_gold = Position(-1, -1);
};

#endif // BOARD_H