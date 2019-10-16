//
//  Board.cpp
//  
//
//  Created by Jonathan Redwine on 10/9/19.
//

#include "Board.h"
#include "util.h"

Board::Board()
{

}

void Board::display_board()
{
    util::display_board(m_map, rows, cols);
}

void Board::generate_new_board(int rows, int cols)
{
    // TODO
}
