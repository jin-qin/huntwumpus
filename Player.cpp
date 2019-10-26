/**
 * @author Jonathan Redwine
 */

#include "Player.h"
#include "KnowledgeBase.h"
#include "util.h"
#include <cmath>
#include <queue>
#include <functional>
#include <algorithm>
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
}

int Player::select_move() {
    // if there are no adjacent dangers
    // if (!knownBoard[currPosX][currPosY].is_breezy() && !knownBoard[currPosX][currPosY].is_smelly()) {
    //     thisMove = find_tile_not_yet_visited(possibleMoves);
    // }
    
    // move(thisMove);
    // return thisMove;

	return 0;
}

void Player::rotate_to(MoveDirection md) {
    auto degree_need_to_rotate = abs((get_degree_by_direction(md) - m_curr_degree));
    degree_need_to_rotate = degree_need_to_rotate > 180 ? 360 - degree_need_to_rotate : degree_need_to_rotate;
    m_score -= degree_need_to_rotate / 90;
    m_curr_degree = get_degree_by_direction(md);
}

void Player::shoot(MoveDirection md) {
    if (m_arrow_throwed) return;

    // rotate first
    rotate_to(md);

    // try to kill wumpus
    auto board = m_board.lock();
    board->try_kill_wumpus(m_curr_pos, md);

    // update knowledge base to mark wumpus is killed.
    // TO DO

    m_score -= 11; // -1 for action shoot, -10 for using up arrows.
}

void Player::grab_gold() {
    // TO DO

    m_score -= 1;
}
void Player::climb_out() {
    // TO DO

    m_score += 999; // +1000 for climbing out the cave with the gold, -1 for action climb.
}

void Player::back_to_entrance() {
    // should calculate a best route, i.e. minimum cost.
    // here we perform A* algorithm to calculate the shortest path.

    auto path_map = calc_best_path();
    auto pos= path_map[Position(0, 0)];
    std::vector<Position> shortest_path;
    while(path_map[pos] != Position(-1, -1)) {
        shortest_path.push_back(pos);
        pos = path_map[pos];
    }
    std::reverse(shortest_path.begin(), shortest_path.end());

    for (int i = 0; i < shortest_path.size(); i++) {
        auto pos = shortest_path[i];
        auto md = get_direction_by_pos(pos);
        rotate_to(md);
        move(md);
    }
}

Player::PathMap Player::calc_best_path() {
    using namespace std;

    if (!m_kb) return PathMap();
    
    auto board = m_board.lock();
    if (!board) return PathMap();

    auto known_map = m_kb->known_map();
    
    // perform A*
    priority_queue<Position, vector<Position>> frontier;
    frontier.push(Position(m_curr_pos));

    auto goal = Position(0, 0); // goal is the entrance.
    PathMap came_from;
    CostMap cost_so_far;
    came_from[m_curr_pos] = Position(-1, -1);
    cost_so_far[m_curr_pos] = 0;
    
    while (!frontier.empty()) {
        auto current = frontier.top();
        frontier.pop();

        if (goal == current)
            break;
        
        auto nbs = available_neighbors(current);
        for (int i = 0; i < nbs.size(); i++) {
            auto nb = nbs[i];
            auto new_cost = cost_so_far[current] + board->cost(current, nb) + rotate_cost(current, nb);
            if (cost_so_far.find(nb) == cost_so_far.end() ||
                new_cost < cost_so_far[nb]) {
                cost_so_far[nb] = new_cost;
                nb.priority = new_cost + heuristic(goal, nb);
                frontier.push(nb);
                came_from[nb] = current;
            }
        }
    }

    return came_from;
}

int Player::rotate_cost(const Position &pos_from, const Position &pos_to) {
    auto md = get_direction_by_pos(pos_from, pos_to);
    auto degree_need_to_rotate = abs((get_degree_by_direction(md) - m_curr_degree));
    degree_need_to_rotate = degree_need_to_rotate > 180 ? 360 - degree_need_to_rotate : degree_need_to_rotate;

    return degree_need_to_rotate;
}

int Player::heuristic(const Position &pos1, const Position &pos2) {
    return abs(pos1.row - pos2.row) + abs(pos1.col - pos2.col);
}

NeighborsList Player::available_neighbors(const Position &pos) {
    auto board = m_board.lock();
    if (!board || !m_kb) return NeighborsList();
    
    auto nbs = util::neighbors(board->rows(), board->cols(), pos);

    for (auto it = nbs.begin(); it != nbs.end();) {
        if (!m_kb->is_safe(*it)) {
            nbs.erase(it);
        }
        else
            ++it;
    }

    return nbs;
}

int Player::find_tile_not_yet_visited(int possibleMoves[4]) {
    // if (possibleMoves[0] != -1) { // check if north tile has been visited
    //     if (!knownBoard[currPosX][currPosY-1].player_been_here()) {
    //         return possibleMoves[0];
    //     }
    // }
    // if (possibleMoves[1] != -1) { // check if east tile has been visited
    //     if (!knownBoard[currPosX+1][currPosY].player_been_here()) {
    //         return possibleMoves[1];
    //     }
    // }
    // if (possibleMoves[2] != -1) { // check if south tile has been visited
    //     if (!knownBoard[currPosX][currPosY+1].player_been_here()) {
    //         return possibleMoves[2];
    //     }
    // }
    // if (possibleMoves[3] != -1) { // check is west tile has been visited
    //     if (!knownBoard[currPosX-1][currPosY].player_been_here()) {
    //         return possibleMoves[3];
    //     }
    // }
    // for (int i = 0; i < 4; i++) {
    //     if (possibleMoves[i] != -1) {
    //         return possibleMoves[i];
    //     }
    // }

	return 0;
}

int Player::get_degree_by_direction(MoveDirection md) {
	switch (md) {
	case MD_UNKNOWN:
		std::cout << __FUNCTION__ << ":: wrong direction, pls check your codes!" << std::endl;
		return 0;
	case MD_NORTH:
		return 0;
	case MD_EAST:
		return 90;
	case MD_SOUTH:
		return 180;
	case MD_WEST:
		return 270;
	default:
		std::cout << __FUNCTION__ << ":: wrong direction, pls check your codes!" << std::endl;
		return 0;
	}
}

MoveDirection Player::get_direction_by_pos(const Position &pos) {
    return get_direction_by_pos(m_curr_pos, pos);
}

MoveDirection Player::get_direction_by_pos(const Position &pos_from, const Position &pos_to) {
    if (pos_to.row - pos_from.row > 0)
        return MD_SOUTH;
    else
        return MD_NORTH;

    if (pos_to.col - pos_from.col > 0)
        return MD_EAST;
    else
        return MD_WEST;

    return MD_UNKNOWN;
}
