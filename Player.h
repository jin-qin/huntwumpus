/**
 * @author Jonathan Redwine
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include "Tile.h"
#include "Board.h"
#include <memory>
#include <unordered_map>

class Board;
class KnowledgeBase;

class Player {
    typedef std::unordered_map<Position, Position> PathMap;
    typedef std::unordered_map<Position, int> CostMap;

public:
    Player(std::weak_ptr<Board> board);

    inline Position curr_pos() { return m_curr_pos; }

    int select_move();

private:
    void init_kb(); // init knowledge base.

    // actions
    void move(MoveDirection md);
    /**
     * @brief each rotation can only rotate 90 degrees.
     * each rotation will make score -= 1.
     * @param md, rotation from current direction to the specified direction md.
     */ 
    void rotate_to(MoveDirection md);
    /**
     * @brief can only use one arrow in each game round, and score will minus 11 after this action.
     */
    void shoot(MoveDirection md);
    void grab_gold();
    void climb_out();
    void back_to_entrance();

    int find_tile_not_yet_visited(int possibleMoves[4]);

    int get_degree_by_direction(MoveDirection md);

    MoveDirection get_direction_by_pos(const Position &pos_from, const Position &pos_to);
    MoveDirection get_direction_by_pos(const Position &pos);

    /**
     * @brief perform A* algorithm to get the mininum cost path.
     */
    PathMap calc_best_path();
    int rotate_cost(const Position &pos_from, const Position &pos_to);
    int heuristic(const Position &pos1, const Position &pos2);

private:
    std::weak_ptr<Board> m_board; // this is not cheating, since Board class does not provide map info.
    std::shared_ptr<KnowledgeBase> m_kb; // to store knowledge base object.

    int m_score = 0;
    int m_curr_degree = 90; // initial degree is 90 degree, means direction face to the right (i.e. east).
                            // 0 <-> north, 90 <-> east, 180 <-> south, 270 <-> west.

    Position m_curr_pos;

    bool m_arrow_throwed = false;
};

#endif //PLAYER_H