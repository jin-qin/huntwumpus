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
    // if (!knownBoard[currPosX][currPosY].is_breezy() && !knownBoard[currPosX][currPosY].is_smelly()) {
    //     thisMove = find_tile_not_yet_visited(possibleMoves);
    // }
    
    // move(thisMove);
    // return thisMove;
}

void Player::rotate_to(MoveDirection md) {
    auto degree_need_to_rotate = abs((get_degree_by_direction(md) - m_curr_degree));
    degree_need_to_rotate = degree_need_to_rotate > 180 ? 360 - degree_need_to_rotate : degree_need_to_rotate;
    m_score -= degree_need_to_rotate / 90;
    m_curr_degree = get_degree_by_direction(md);
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
}
