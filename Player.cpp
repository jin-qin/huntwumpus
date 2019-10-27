/**
 * @author Jonathan Redwine
 */

#include "Player.h"
#include "KnowledgeBase.h"
#include "util.h"
#include <cmath>
#include <functional>
#include <random>
#include <iostream>

Player::Player(std::weak_ptr<Board> board)
    : m_board(board)
{
    init_kb();
}

void Player::init_kb() {
    auto board = m_board.lock();
    if (!board) {
        std::cout << __FUNCTION__ << ":: fatal error, board should not be null!" << std::endl;
        return;
    }

    m_kb = std::make_shared<KnowledgeBase>(board->rows(), board->cols());

    // initial knowledge
    m_kb->add_knowledge(m_curr_pos, *(current_tile()));
}

void Player::move(MoveDirection md) {
    auto board = m_board.lock();
    if (!board) return;

    switch (md) {
        case MD_NORTH: {
            if (m_curr_pos.row <= 0)
                return;
            m_curr_pos.row -= 1;
            break;
        }
        case MD_EAST: {
            if (m_curr_pos.col >= board->cols() - 1)
                return;
            m_curr_pos.col += 1;
            break;
        }
        case MD_SOUTH: {
            if (m_curr_pos.row >= board->rows() - 1)
                return;
            m_curr_pos.row += 1;
            break;
        }
        case MD_WEST: {
            if (m_curr_pos.col <= 0)
                return;
            m_curr_pos.col -= 1;
            break;
        }
        default:
            std::cout << __FUNCTION__ << ":: wrong move direction, pls check codes!" << std::endl;
            return;
    }
    m_score -= 1;

    // calculate rotate after the move action.
    // do not try to put it before move action, if you cannot move, then you should not rotate.
    rotate_to(md);

    check_move_result();

    add_new_knowledge();
}

PositionList Player::select_move() {
    auto best_mv_path = best_move(m_curr_pos);
    if (best_mv_path.empty()) {
        std::cout << __FUNCTION__ << ":: error: cannot find an available move!" << std::endl;
        return best_mv_path;
    }

    for (int i = 0; i < best_mv_path.size(); i++)
        move(util::next_direction(m_curr_pos, best_mv_path[i]));

    return best_mv_path;
}

PositionList Player::best_move(const Position &pos) {
    if (!m_kb) return PositionList();

    auto safe_nbs = m_kb->safe_neighbors(m_curr_pos);
    if (safe_nbs.size() > 0) {
        std::cout << __FUNCTION__ << ":: safe_neighbors: " << safe_nbs.size() << std::endl;
        return select_best_move(safe_nbs);
    }

    auto safe_his_nbs = m_kb->safe_history_neighbors(m_curr_pos);
    if (safe_his_nbs.size() > 0) {
        std::cout << __FUNCTION__ << ":: safe_history_neighbors: " << safe_his_nbs.size() << std::endl;
        return select_best_move(safe_his_nbs);
    }
    
    auto avail_nbs = m_kb->available_neighbors(m_curr_pos);
    if (avail_nbs.size() > 0) {
        std::cout << __FUNCTION__ << ":: available_neighbors: " << avail_nbs.size() << std::endl;
        return select_best_move(avail_nbs);
    }
    
    auto avail_his_nbs = m_kb->available_history_neighbors(m_curr_pos);
    if (avail_his_nbs.size() > 0) {
        std::cout << __FUNCTION__ << ":: available_history_neighbors: " << avail_his_nbs.size() << std::endl;
        return select_best_move(avail_his_nbs);
    }

    std::cout << __FUNCTION__ << ":: " << "no available moves!" << std::endl;

    return PositionList();
}

PositionList Player::select_best_move(const PositionList &pref_mvs) {
    PositionList mv_path;

    int min_cost = INT32_MAX;
    for (int i = 0; i < pref_mvs.size(); i++) {
        PositionList mv_path_tmp;
        int cost = util::min_cost(*m_kb, m_curr_pos, pref_mvs[i], m_curr_degree, mv_path_tmp);
        // std::cout << __FUNCTION__ << ":: min cost:" << cost << std::endl;
        if (min_cost > cost) {
            min_cost = cost;
            mv_path = mv_path_tmp;
        }
    }
    return mv_path;
}

void Player::rotate_to(MoveDirection md) {
    auto degree_new = util::degree_by_direction(md);
    auto degree_rotate = abs((degree_new - m_curr_degree));
    degree_rotate = degree_rotate > 180 ? 360 - degree_rotate : degree_rotate;
    m_score -= degree_rotate / 90;
    m_curr_degree = degree_new;
}

void Player::shoot(MoveDirection md) {
    if (m_arrow_throwed) return;
    m_arrow_throwed = true;

    // rotate first
    rotate_to(md);

    // try to kill wumpus
    auto board = m_board.lock();
    m_wumpus_killed = board->try_kill_wumpus(m_curr_pos, md);

    // update knowledge base to mark wumpus is killed.
    if (!m_kb) return;
    if (m_wumpus_killed) m_kb->set_wumpus_killed();

    m_score -= 11; // -1 for action shoot, -10 for using up arrows.

    check_mission_compelete();
}

void Player::grab_gold() {
    m_has_gold = true;

    m_score -= 1;
}
void Player::climb_out() {
    if (m_curr_pos != Position(0, 0)) {
        std::cout << __FUNCTION__ << ":: failed, your position is not in the start, pls check your codes!" << std::endl;
        return;
    }

    m_score += 999; // +1000 for climbing out the cave with the gold, -1 for action climb.

    on_game_over();
}

void Player::back_to_entrance() {
    // should calculate a best route, i.e. minimum cost.
    // here we perform A* algorithm to calculate the shortest path.

    if (!m_kb) return;

    auto result = util::calc_best_path(*m_kb, m_curr_pos, Position(0, 0), m_curr_degree);
    auto best_path = result.first;
    for (int i = 0; i < best_path.size(); i++) {
        auto pos = best_path[i];
        auto md = next_direction(pos);
        move(md);
    }
}

MoveDirection Player::next_direction(const Position &pos_to) {
    return util::next_direction(m_curr_pos, pos_to);
}

void Player::add_new_knowledge() {
    auto curr_tile = current_tile();
    m_kb->add_knowledge(m_curr_pos, *curr_tile);
}

void Player::check_move_result() {
    auto curr_tile = current_tile();

    // check died
    if (curr_tile->mustbe_wumpus() ||
        curr_tile->mustbe_pit()) {
        m_score -= 1000;
        on_game_over();
        return;
    }

    // check grab gold
    if (curr_tile->has_gold()) {
        grab_gold();
        on_grab_gold();
    }
}

void Player::on_grab_gold() {
    check_mission_compelete();
}

bool Player::mission_complete() {
    if (game_mode_get_gold_only() && m_has_gold)
        return true;
    if (game_mode_kill_wumpus_only() && m_wumpus_killed)
        return true;
    if (game_mode_both() &&
        m_has_gold &&
        m_wumpus_killed)
        return true;
    
    return false;
}

void Player::check_mission_compelete() {
    if (mission_complete()) {
        back_to_entrance();
        climb_out();
        on_game_over();
    }
}

void Player::on_game_over() {
    m_game_over = true;
}

std::shared_ptr<Tile> Player::current_tile() {
    auto board = m_board.lock();
    if (!board) return 0;
    return board->tile(m_curr_pos);
}

bool Player::game_mode_get_gold_only() {
    return (0 != (m_game_mode & GM_GET_GOLD)) && (0 == (m_game_mode & GM_KILL_WUMPUS));
}

bool Player::game_mode_kill_wumpus_only() {
    return (0 != (m_game_mode & GM_KILL_WUMPUS)) && (0 == (m_game_mode & GM_GET_GOLD));
}

bool Player:: game_mode_both() {
    return (0 != (m_game_mode & GM_GET_GOLD)) && (0 != (m_game_mode & GM_KILL_WUMPUS));
}