#ifndef Knowledge_H
#define Knowledge_H

#include "Board.h"
#include <vector>

class Tile;

class KnowledgeBase{    
public:
    KnowledgeBase(int row_len, int col_len);
	~KnowledgeBase();

    inline Board::Map& known_map() {return m_map;}
    inline std::vector<Position>& history() {return m_history_pos;}
    void add_knowledge(int row, int col, Tile status);
    void create_knowledge(int rows, int cols);
    void analyze_knowledge(int cur_row, int cur_col);
    void update_knowledge();
    bool judge_edge_status(int row, int col);
    bool judge_normal_status(int row, int col);

private:
    void add_knowledge_into_history(int row, int col);

private:
    int m_row_len = 0, m_col_len = 0, m_counter = 0;
    Board::Map m_map; //Store information of board

    std::vector<Position> m_history_pos; // History positions
};


#endif