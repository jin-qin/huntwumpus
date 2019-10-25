/**
 * @author Jonathan Redwine
 */

#include "Board.h"
#include "Tile.h"
#include "util.h"

#include <iostream>
#include <random>

Board::Board() {
}

Board::~Board() {
}

void Board::display_board() {
    util::display_board(m_map);
}

Board::NeighborsList Board::neighbors(const Position &pos) {
    auto row = pos.row;
    auto col = pos.col;
    auto nbs = NeighborsList();

    if (col > 0)
        nbs.push_back(Position(row, col - 1));
    if (row > 0)
        nbs.push_back(Position(row - 1, col));
    if (col < m_cols - 1)
        nbs.push_back(Position(row, col + 1));
    if (row < m_rows - 1)
        nbs.push_back(Position(row + 1, col));
    return nbs;
}

int Board::cost(const Position &pos1, const Position &pos2) {
    auto nbs1 = neighbors(pos1);
    for (int i = 0; i < nbs1.size(); i++)
        if (nbs1[i].row == pos2.row && nbs1[i].col == pos2.col)
            return 1;

    return -1;
}

void Board::generate_new_map(int rows, int cols) {
    cleanup();

    m_map = util::create_new_map(rows, cols);
    m_rows = rows;
    m_cols = cols;

    std::random_device rd;
    // generate gold
    auto ind_gold = 0;
    while (ind_gold == 0) {
        ind_gold = rd() % (rows * cols);
    }
    m_pos_gold.row = ind_gold / cols;
    m_pos_gold.col = ind_gold % cols;
    m_map[m_pos_gold.row][m_pos_gold.col]->add_state(Tile::TS_GOLD);

    // generate wumpus
    auto ind_wumpus = 0;
    while (ind_wumpus == ind_gold || ind_wumpus == 0) {
        ind_wumpus = rd() % (rows * cols);
    }
    m_pos_wumpus.row = ind_wumpus / cols;
    m_pos_wumpus.col = ind_wumpus % cols;
    m_map[m_pos_wumpus.row][m_pos_wumpus.col ]->add_state(Tile::TS_WUMPUS);
    generate_smelly_of_wumpus(m_pos_wumpus);
    
    // generate pits.
    for (size_t i = 0; i < m_map.size(); i++) {
        for (size_t j = 0; j < m_map[i].size(); j++) {
            if (!m_map[i][j]) {
                std::cout << __FUNCTION__ << "::fatal error, empty pointer of the map, this should not appear!" << std::endl;
                return;
            }

            if (m_map[i][j]->is_entrance() ||
                m_map[i][j]->has_gold() ||
                m_map[i][j]->has_wumpus())
                continue;
            
            auto prob = rd() % 10 + 1;
            if (prob <= 2) {
                m_map[i][j]->remove_state(Tile::TS_BREEZY);
                m_map[i][j]->remove_state(Tile::TS_SMELLY);

                m_map[i][j]->add_state(Tile::TS_PIT);
                generate_breezy_of_pit(Position(i, j));
            }
        }
    }
    
}

void Board::generate_breezy_of_pit(const Position &pos) {
    auto row = pos.row;
    auto col = pos.col;
    if (row < 0 || row >= m_rows || 
        col < 0 || col >= m_cols ||
        (row == 0 && col == 0)) {
        std::cout << __FUNCTION__ << "::wrong row or col, pls check codes!" << std::endl;
        return;
    }

    auto nbs = neighbors(pos);
    for (int i = 0; i < nbs.size(); i++) {
        auto adj_row = nbs[i].row;
        auto adj_col = nbs[i].col;
        if (!m_map[adj_row][adj_col]->has_pit())
            m_map[adj_row][adj_col]->add_state(Tile::TS_BREEZY);
    }
}

void Board::generate_smelly_of_wumpus(const Position &pos) {
    auto row = pos.row;
    auto col = pos.col;
    if (row < 0 || row >= m_rows || 
        col < 0 || col >= m_cols ||
        (row == 0 && col == 0)) {
        std::cout << __FUNCTION__ << "::wrong row or col, pls check codes!" << std::endl;
        return;
    }

    auto nbs = neighbors(pos);
    for (int i = 0; i < nbs.size(); i++)
        m_map[nbs[i].row][nbs[i].col]->add_state(Tile::TS_SMELLY);
}

void Board::cleanup() {
    m_rows = 0;
    m_cols = 0;

    m_pos_wumpus = Position(-1, -1);
    m_pos_gold = Position(-1, -1);
}

bool Board::try_kill_wumpus(const Position &pos, MoveDirection md) {
    auto row = pos.row;
    auto col = pos.col;
    auto row_wumpus = m_pos_wumpus.row;
    auto col_wumpus = m_pos_wumpus.col;
    bool wumpus_killed = false;
    if (row == row_wumpus &&
        (md == MD_WEST || md == MD_EAST)) {
        if (((col_wumpus - col > 0) && (md == MD_EAST)) || 
            ((col_wumpus - col < 0) && (md == MD_WEST)))
            wumpus_killed = true;
    }

    if (col == col_wumpus &&
        (md == MD_NORTH || md == MD_SOUTH)) {
        if (((row_wumpus - row > 0) && (md == MD_NORTH)) ||
            ((row_wumpus - row < 0) && (md == MD_SOUTH)))
            wumpus_killed = true;
    }

    if (wumpus_killed)
        clear_smelly_and_wumpus();

    return wumpus_killed;
}

void Board::clear_smelly_and_wumpus() {
    auto nbs = neighbors(m_pos_wumpus);
    for (int i = 0; i < nbs.size(); i++)
        m_map[nbs[i].row][nbs[i].col]->remove_state(Tile::TS_SMELLY);
    
    m_pos_wumpus = Position(-1, -1);
}