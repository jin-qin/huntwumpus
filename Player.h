/**
 * @author Jonathan Redwine
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include "Tile.h"

class Board;

class Player {
    enum MoveDirection {
        MD_UNKNOWN = 0,
        MD_NORTH,
        MD_EAST,
        MD_SOUTH,
        MD_WEST
    };

public:
    Tile **knownBoard = 0; // TODO

    Player();

    inline int curr_pos_row() { return m_curr_pos_row; }
    inline int curr_pos_col() { return m_curr_pos_col; }

    // actions
    void move(MoveDirection md);
    int select_move();

    /**
     * @brief each rotation can only rotate 90 degrees.
     * each rotation will make score -= 1.
     * @param md, rotation from current direction to the specified direction md.
     */ 
    void rotate_to(MoveDirection md);

    int find_tile_not_yet_visited(int possibleMoves[4]);

private:
    int get_degree_by_direction(MoveDirection md);

private:
    Board m_board;
    int m_score = 0;
    int m_curr_degree = 90; // initial degree is 90 degree, means direction face to the right (i.e. east).
                            // 0 <-> north, 90 <-> east, 180 <-> south, 270 <-> west.

    int m_curr_pos_row = 0;
    int m_curr_pos_col = 0;
};

#endif //PLAYER_H