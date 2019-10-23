/**
 * @author Jonathan Redwine
 */

#include "Player.h"
#include <cmath>
#include <iostream>
using namespace std;

Player::Player() {
}

void Player::set_board(std::weak_ptr<Board> board) {
    m_board = board;
}

void Player::move(MoveDirection md) {
    auto board = m_board.lock();
    if (!board) return;

    switch (md) {
        case MD_NORTH: {
            if (m_curr_pos_row <= 0)
                return;
            m_curr_pos_row -= 1;
            break;
        }
        case MD_EAST: {
            if (m_curr_pos_col >= board->cols() - 1)
                return;
            m_curr_pos_col += 1;
            break;
        }
        case MD_SOUTH: {
            if (m_curr_pos_row >= board->rows() - 1)
                return;
            m_curr_pos_row += 1;
            break;
        }
        case MD_WEST: {
            if (m_curr_pos_col <= 0)
                return;
            m_curr_pos_col -= 1;
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
    if ((m_kb.m_map[m_curr_pos_row][m_curr_pos_col].isSmelly() == false) && (m_kb.m_map[m_curr_pos_row][m_curr_pos_col].isBreezy() == false)) {
        thisMove = find_tile_not_yet_visited();
    }
    
    move(thisMove);
    return thisMove;

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

    // try to kill wumpus
    auto board = m_board.lock();
    board->try_kill_wumpus(m_curr_pos_row, m_curr_pos_col, md);

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
    // should calculate a best route.
    // TO DO
}




int Player::find_tile_not_yet_visited() {
    if (m_curr_degree == 90) { // if facing east
        if (m_curr_col != (m_kb.m_map[m_curr_row]).size() - 1) { // if we're not at east edge of board
            if (m_kb.m_map[m_curr_row][m_curr_col+1].playerBeenHere() == false) { // and east tile not yet visited
                m_curr_degree = 90;
                return m_curr_degree;
            }
        } else if (m_curr_row != (m_kb.m_map).size() - 1) { // if we're not at south edge of board
            if (m_kb.m_map[m_curr_row+1][m_curr_col].playerBeenHere() == false) { // and south tile is not visited
                m_curr_degree = 180;
                return m_curr_degree;
            }
        } else if (m_curr_row != 0) { // if we're not at north edge of board
            if (m_kb.m_map[m_curr_row-1][m_curr_col].playerBeenHere() == false) { // and north tile not yet visited
                m_curr_degree = 0;
                return m_curr_degree; // move north if north tile not yet visited
            }
        } else if (m_curr_col != 0) { // if we're not at west edge of board
            if (m_kb.m_map[m_curr_row][m_curr_col-1].playerBeenHere() == false) { // and west tile not yet visited
                m_curr_degree = 270;
                return m_curr_degree;
            }
        }
    } else if (m_curr_degree == 180) { // if facing south
        if (m_curr_row != (m_kb.m_map).size() - 1) { // if we're not at south edge of board
            if (m_kb.m_map[m_curr_row+1][m_curr_col].playerBeenHere() == false) { // and south tile is not visited
                m_curr_degree = 180;
                return m_curr_degree;
            }
        } else if (m_curr_col != (m_kb.m_map[m_curr_row]).size() - 1) { // if we're not at east edge of board
            if (m_kb.m_map[m_curr_row][m_curr_col+1].playerBeenHere() == false) { // and east tile not yet visited
                m_curr_degree = 90;
                return m_curr_degree;
            }
        } else if (m_curr_col != 0) { // if we're not at west edge of board
            if (m_kb.m_map[m_curr_row][m_curr_col-1].playerBeenHere() == false) { // and west tile not yet visited
                m_curr_degree = 270;
                return m_curr_degree;
            }
        } else if (m_curr_row != 0) { // if we're not at north edge of board
            if (m_kb.m_map[m_curr_row-1][m_curr_col].playerBeenHere() == false) { // and north tile not yet visited
                m_curr_degree = 0;
                return m_curr_degree; // move north if north tile not yet visited
            }
        }
    } else if (m_curr_degree == 270) { // if facing west
        if (m_curr_col != 0) { // if we're not at west edge of board
            if (m_kb.m_map[m_curr_row][m_curr_col-1].playerBeenHere() == false) { // and west tile not yet visited
                m_curr_degree = 270;
                return m_curr_degree;
            }
        } else if (m_curr_row != (m_kb.m_map).size() - 1) { // if we're not at south edge of board
            if (m_kb.m_map[m_curr_row+1][m_curr_col].playerBeenHere() == false) { // and south tile is not visited
                m_curr_degree = 180;
                return m_curr_degree;
            }
        } else if (m_curr_row != 0) { // if we're not at north edge of board
            if (m_kb.m_map[m_curr_row-1][m_curr_col].playerBeenHere() == false) { // and north tile not yet visited
                m_curr_degree = 0;
                return m_curr_degree; // move north if north tile not yet visited
            }
        } else if (m_curr_col != (m_kb.m_map[m_curr_row]).size() - 1) { // if we're not at east edge of board
            if (m_kb.m_map[m_curr_row][m_curr_col+1].playerBeenHere() == false) { // and east tile not yet visited
                m_curr_degree = 90;
                return m_curr_degree;
            }
        }
    } else if (m_curr_degree == 0) { // if facing north
        if (m_curr_row != 0) { // if we're not at north edge of board
            if (m_kb.m_map[m_curr_row-1][m_curr_col].playerBeenHere() == false) { // and north tile not yet visited
                m_curr_degree = 0;
                return m_curr_degree; // move north if north tile not yet visited
            }
        } else if (m_curr_col != (m_kb.m_map[m_curr_row]).size() - 1) { // if we're not at east edge of board
            if (m_kb.m_map[m_curr_row][m_curr_col+1].playerBeenHere() == false) { // and east tile not yet visited
                m_curr_degree = 90;
                return m_curr_degree;
            }
        } else if (m_curr_col != 0) { // if we're not at west edge of board
            if (m_kb.m_map[m_curr_row][m_curr_col-1].playerBeenHere() == false) { // and west tile not yet visited
                m_curr_degree = 270;
                return m_curr_degree;
            }
        } else if (m_curr_row != (m_kb.m_map).size() - 1) { // if we're not at south edge of board
            if (m_kb.m_map[m_curr_row+1][m_curr_col].playerBeenHere() == false) { // and south tile is not visited
                m_curr_degree = 180;
                return m_curr_degree;
            }
        }
    }
	return -1; // return -1 if no adjacent tile is unvisited
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
