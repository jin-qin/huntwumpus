/**
 * @author Jonathan Redwine
 */

#include "Board.h"
#include "util.h"

Board::Board()
{

}

Board::~Board()
{
    util::destroy_map(m_map);
}

void Board::display_board()
{
    util::display_board(m_map, m_rows, m_cols);
}

void Board::generate_new_map(int rows, int cols)
{
    // make sure the previous map is destroyed.
    util::destroy_map(m_map);

    m_map = util::create_new_map(rows, cols);
}
