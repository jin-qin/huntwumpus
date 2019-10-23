#ifndef Knowledge_H
#define Knowledge_H

#include "Board.h"

class Tile;

class KnowledgeBase{
public:
    KnowledgeBase(int row_len, int col_len);
	~KnowledgeBase();

    void add_knowledge(int row_x, int col_y, Tile status);
    void create_knowledge(int rows, int cols);
    void analyze(int cur_x, int cur_y);
    void update_knowledge();
    void judge_status();


private:
    int m_row_len = 0, m_col_len = 0, m_counter = 0;
    Board::Map m_map; //Store information of board
};


#endif