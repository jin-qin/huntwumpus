/**
 * @author Jonathan Redwine
 */

#include "Board.h"
#include "Tile.h"
#include "util.h"

#include "time.h"
#include <iostream>

Board::Board() {
}

Board::~Board() {
}

void Board::display_board() {
    util::display_board(m_map);
}

void Board::generate_new_map(int rows, int cols) {
    cleanup();

    m_map = util::create_new_map(rows, cols);
    m_rows = rows;
    m_cols = cols;

    // generate gold
    srand(time(NULL) / 2);
    auto ind_gold = rand() % (rows * cols);
    m_row_gold = ind_gold / cols;
    m_col_gold = ind_gold % cols;
    m_map[m_row_gold][m_col_gold]->add_state(Tile::TS_GOLD);

    // generate wumpus
    srand(time(NULL) / 3);
    auto ind_wumpus = -1;
    while (ind_wumpus != ind_gold) {
        ind_wumpus = rand() % (rows * cols);
        srand(time(NULL) - ind_wumpus);
    }
    m_row_wumpus = ind_wumpus / cols;
    m_col_wumpus = ind_wumpus % cols;
    m_map[m_row_wumpus][m_col_wumpus]->add_state(Tile::TS_WUMPUS);
    generate_smelly_of_wumpus(m_row_wumpus, m_col_wumpus);
    
    // generate pits.
    for (size_t i = 0; i < m_map.size(); i++) {
        for (size_t j = 0; j < m_map[i].size(); j++) {
            if (!m_map[i][j]) {
                std::cout << __FUNCTION__ << "::fatal error, empty pointer of the map, this should not appear!" << std::endl;
                return;
            }

            if (m_map[i][j]->is_entrance())
                continue;
            
            srand(time(NULL) + j);
            auto prob = rand() % 10 + 1;
            if (prob <= 2) {
                m_map[i][j]->add_state(Tile::TS_PIT);
                generate_breezy_of_pit(i, j);
            }
        }
    }
    
}

void Board::generate_breezy_of_pit(int row, int col) {
    if (row < 0 || row >= m_rows || 
        col < 0 || col >= m_cols ||
        (row == 0 && col == 0)) {
        std::cout << __FUNCTION__ << "::wrong row or col, pls check codes!" << std::endl;
        return;
    }

    if (col > 0)
        m_map[row][col - 1]->add_state(Tile::TS_BREEZY);
    if (row > 0)
        m_map[row - 1][col]->add_state(Tile::TS_BREEZY);
    if (col < m_cols - 1)
        m_map[row][col + 1]->add_state(Tile::TS_BREEZY);
    if (row < m_rows - 1)
        m_map[row + 1][col]->add_state(Tile::TS_BREEZY);
}

void Board::generate_smelly_of_wumpus(int row, int col) {
    if (row < 0 || row >= m_rows || 
        col < 0 || col >= m_cols ||
        (row == 0 && col == 0)) {
        std::cout << __FUNCTION__ << "::wrong row or col, pls check codes!" << std::endl;
        return;
    }

    if (col > 0)
        m_map[row][col - 1]->add_state(Tile::TS_SMELLY);
    if (row > 0)
        m_map[row - 1][col]->add_state(Tile::TS_SMELLY);
    if (col < m_cols - 1)
        m_map[row][col + 1]->add_state(Tile::TS_SMELLY);
    if (row < m_rows - 1)
        m_map[row + 1][col]->add_state(Tile::TS_SMELLY);
}

void Board::cleanup() {
    m_rows = 0;
    m_cols = 0;
    m_row_wumpus = -1;
    m_col_wumpus = -1;
    m_row_gold = -1;
    m_col_gold = -1;
}

bool Board::try_kill_wumpus(int row, int col, MoveDirection md) {
    bool wumpus_killed = false;
    if (row == m_row_wumpus &&
        (md == MD_WEST || md == MD_EAST)) {
        if ((m_col_wumpus - col > 0) && (md == MD_EAST))
            wumpus_killed = true;
        if ((m_col_wumpus - col < 0) && (md == MD_WEST))
            wumpus_killed = true;
    }

    if (col == m_col_wumpus &&
        (md == MD_NORTH || md == MD_SOUTH)) {
        if ((m_row_wumpus - row > 0) && (md == MD_NORTH))
            wumpus_killed = true;
        if ((m_row_wumpus - row < 0) && (md == MD_SOUTH))
            wumpus_killed = true;
    }

    if (wumpus_killed)
        clear_smelly_and_wumpus();

    return wumpus_killed;
}

void Board::clear_smelly_and_wumpus() {
    if (m_col_wumpus > 0)
        m_map[m_row_wumpus][m_col_wumpus - 1]->remove_state(Tile::TS_SMELLY);
    if (m_row_wumpus > 0)
        m_map[m_row_wumpus - 1][m_col_wumpus]->remove_state(Tile::TS_SMELLY);
    if (m_col_wumpus < m_cols - 1)
        m_map[m_row_wumpus][m_col_wumpus + 1]->remove_state(Tile::TS_SMELLY);
    if (m_row_wumpus < m_rows - 1)
        m_map[m_row_wumpus + 1][m_col_wumpus]->remove_state(Tile::TS_SMELLY);

    m_row_wumpus = -1;
    m_col_wumpus = -1;
}