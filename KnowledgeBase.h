#ifndef Knowledge_H
#define Knowledge_H

#include "Board.h"
#include <vector>

class Tile;

class KnowledgeBase{    
public:
    KnowledgeBase(int row_len, int col_len);
	~KnowledgeBase();

    inline int rows() const { return m_row_len; }
    inline int cols() const { return m_col_len; }
    inline const Board::Map& known_map() const { return m_map; }
    inline const std::vector<Position>& history() const { return m_history_pos; }
    inline const Position& wumpus_pos() const { return m_pos_wumpus; }

    bool is_safe(const Position &pos) const;

    PositionList safe_neighbors(const Position &pos, bool use_history = false) const;
    PositionList safe_history_neighbors(const Position &pos) const;
    PositionList available_neighbors(const Position &pos, bool use_history = false) const;
    PositionList available_history_neighbors(const Position &pos) const;

    void on_wumpus_killed();
    void on_kill_wumpus_failed(const Position &pos, MoveDirection md);

    bool wumpus_found();

    PositionList possible_wumpus_positions();

    void create_knowledge(int rows, int cols);
    void add_knowledge(const Position &pos, Tile status);
    void analyze_knowledge(const Position &pos);

private:
    void add_knowledge_into_history(const Position &pos);

    PositionList unsafe_neighbors(const PositionList &nbs);

    void check_add_safe(const Position &pos);
    void infer_pit_wumpus(const Position &pos);
    void infer_history(const Position &pos);
    void check_found_wumpus(const Position &pos);

    void clear_wumpus_smelly();

    std::shared_ptr<Tile> tile(const Position &pos) const;

    bool in_history(const Position &pos) const;

private:
    int m_row_len = 0, m_col_len = 0;
    Board::Map m_map; //Store information of board

    std::vector<Position> m_history_pos; // History positions

    bool m_wumpus_killed = false;

    Position m_pos_wumpus = Position(-1, -1);
    PositionList m_possible_wumpus_positions;
};


#endif