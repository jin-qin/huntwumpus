/**
 * @author Jonathan Redwine
 */

#include "Board.h"
#include "Tile.h"
#include "util.h"

#include "time.h"
#include <iostream>

Board::Board()
{

}

Board::~Board()
{
    util::destroy_map(m_map);
    m_rows = 0;
    m_cols = 0;
}

void Board::display_board()
{
    util::display_board(m_map);
}

void Board::generate_new_map(int rows, int cols)
{
    // destroy the old map.
    util::destroy_map(m_map);

    m_map = util::create_new_map(rows, cols);
    m_rows = rows;
    m_cols = cols;

    // generate gold
    srand(time(NULL) / 2);
    auto ind = rand() % (rows * cols);
    m_map[ind / cols][ind % cols]->add_state(Tile::TS_GOLD);

    // generate wumpus
    srand(time(NULL) / 3);
    auto ind = rand() % (rows * cols);
    m_map[ind / cols][ind % cols]->add_state(Tile::TS_WUMPUS);
    generate_smelly_of_wumpus(ind / cols, ind % cols);
    
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

void Board::generate_breezy_of_pit(int row, int col)
{
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

void Board::generate_smelly_of_wumpus(int row, int col)
{
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
