/**
 * @author Jonathan Redwine
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include "Tile.h"
#include "Board.h"
#include <memory>

class Board;
class KnowledgeBase;

class Player {
public:
    enum GameMode {
        GM_UNKNOWN      = 0x0000,
        GM_GET_GOLD     = 0x0001,
        GM_KILL_WUMPUS  = 0x0002
    };

public:
    Player(std::weak_ptr<Board> board);

    inline void set_game_mode(GameMode gm) { m_game_mode = gm; }
    inline void set_game_mode(int gm) { m_game_mode = static_cast<GameMode>(gm); }
    inline Position curr_pos() { return m_curr_pos; }
    inline int score() { return m_score; };
    inline bool game_over() { return m_game_over; }
    inline bool wumpus_killed() { return m_wumpus_killed; }
    inline bool has_gold() { return m_has_gold; }

    PositionList select_move();

    void display_knowledgebase();

private:
    void init_kb(); // init knowledge base.

    // actions
    void move(MoveDirection md);
    void move_by_path(const PositionList &path);
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

    PositionList prefer_move(const Position &pos);
    PositionList best_move(const Position &pos);
    PositionList gamble_move(const Position &pos);
    PositionList select_mincost_move(const PositionList &pref_mvs);

    MoveDirection next_direction(const Position &pos_to);

    void add_new_knowledge();

    void check_move_result();

    void on_grab_gold();

    bool mission_complete();
    void check_mission_compelete();
    
    void on_game_over();

    std::shared_ptr<Tile> current_tile();

    bool game_mode_kill_wumpus();
    bool game_mode_get_gold_only();
    bool game_mode_kill_wumpus_only();
    bool game_mode_both();

    bool can_kill_wumpus();
    /**
     * @brief no safe moves, have to try kill the wumpus and get available safe moves.
     * we can get possible wumpus positions from knowledge base and select one randomly.
     */
    bool need_kill_wumpus();
    void try_kill_wumpus();

    Position best_position_to_kill_wumpus(const Position &pos);

private:
    std::weak_ptr<Board> m_board; // this is not cheating, since Board class does not provide map info.
    std::shared_ptr<KnowledgeBase> m_kb; // to store knowledge base object.

    int m_score = 0;
    int m_curr_degree = 90; // initial degree is 90 degree, means direction face to the right (i.e. east).
                            // 0 <-> north, 90 <-> east, 180 <-> south, 270 <-> west.

    Position m_curr_pos = Position(0, 0);

    bool m_arrow_throwed = false;
    bool m_has_gold = false;
    bool m_wumpus_killed = false;

    GameMode m_game_mode = GM_GET_GOLD;

    bool m_game_over = false;
};

#endif //PLAYER_H