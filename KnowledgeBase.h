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
    void add_knowledge(const Position &pos, Tile status);
    void create_knowledge(int rows, int cols);
    void analyze_knowledge(const Position &pos);
    void update_knowledge();

private:
    void add_knowledge_into_history(const Position &pos);

    NeighborsList unsafe_neighbors(const NeighborsList &nbs);

    void check_add_safe(const Position &pos);
    void infer_pit_wumpus(const Position &pos);
    void infer_history(const Position &pos);

private:
    int m_row_len = 0, m_col_len = 0, m_counter = 0;
    Board::Map m_map; //Store information of board

    std::vector<Position> m_history_pos; // History positions
};


#endif