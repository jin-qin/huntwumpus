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

    bool is_safe(const Position &pos);

    void set_wumpus_killed();

    void create_knowledge(int rows, int cols);
    void add_knowledge(const Position &pos, Tile status);
    void analyze_knowledge(const Position &pos);

private:
    void add_knowledge_into_history(const Position &pos);

    NeighborsList unsafe_neighbors(const NeighborsList &nbs);

    void check_add_safe(const Position &pos);
    void infer_pit_wumpus(const Position &pos);
    void infer_history(const Position &pos);

    void clear_wumpus_smelly();

private:
    int m_row_len = 0, m_col_len = 0;
    Board::Map m_map; //Store information of board

    std::vector<Position> m_history_pos; // History positions

    bool m_wumpus_killed = false;
};


#endif