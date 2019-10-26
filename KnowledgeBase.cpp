#include "KnowledgeBase.h"
#include "Tile.h"
#include "common.h"
#include "util.h"
#include <utility>

using namespace std;

KnowledgeBase::KnowledgeBase(int row_len, int col_len)
             :m_row_len(row_len)
             ,m_col_len(col_len)
{ 
    create_knowledge(row_len,col_len);   
}

KnowledgeBase::~KnowledgeBase(){

}
  
void KnowledgeBase::create_knowledge(int rows, int cols){ // Initialize the knowledge board
	m_map = util::create_new_map(rows, rows);
}

void KnowledgeBase::add_knowledge(const Position &pos, Tile status){ // Receive the new information and add to current map
    //if there are pit, wumpus, gold, the game is over. Therefore in this case, the program will not call this func.
    //So we just need to think about breezy and smelly.

    int row = pos.row;
    int col = pos.col;
    if(status.is_entrance()){
        m_map[row][col]->add_state(Tile::TS_ENTRANCE);
    }

    using namespace std;
    vector< pair<Tile::TileState, Tile::TileState> > target_states;

    if (status.is_breezy()) target_states.push_back(make_pair(Tile::TS_BREEZY, Tile::TS_PIT));
    if (status.is_smelly()) target_states.push_back(make_pair(Tile::TS_SMELLY, Tile::TS_WUMPUS));
    
    for (int i = 0; i < target_states.size(); i++) {
        m_map[row][col]->add_state(target_states[i].first);
        auto nbs = util::neighbors(m_row_len, m_col_len, Position(row, col));
        for (int j = 0; j < nbs.size(); j++){
            if(!m_map[nbs[j].row][nbs[j].col]->is_safe()){
                m_map[nbs[j].row][nbs[j].col]->add_state(target_states[i].second);
            }
        }
    }

    m_map[row][col]->remove_state(Tile::TS_PIT);
    m_map[row][col]->remove_state(Tile::TS_WUMPUS);
    m_map[row][col]->add_state(Tile::TS_SAFE);

    add_knowledge_into_history(pos);
    //To continue
    analyze_knowledge(pos);
};

void KnowledgeBase::add_knowledge_into_history(const Position &pos) {
    auto row = pos.row;
    auto col = pos.col;
    for (int i = 0; i < m_history_pos.size(); i++) {
        if (m_history_pos[i].row == row && m_history_pos[i].col == col)
            return;
    }

    m_history_pos.push_back(pos);
}

void KnowledgeBase::analyze_knowledge(const Position &pos) {//use the current information to infer the known space
    //rule: if 3 of 4 points around the position which is smelly or breezy is safe, the remaining point is the pit/wumpus.
    //search rule: searching from current position to the first position by using array m_row and m_pos_y.
    int row = pos.row;
    int col = pos.col;
    
    check_add_safe(pos);

    infer_pit_wumpus(pos);

    infer_history(pos);
}

NeighborsList KnowledgeBase::unsafe_neighbors(const NeighborsList &nbs) {
    NeighborsList unsafe_nbs();
    for (int i = 0; i < nbs.size(); i++) {
        if (!nbs[i]->is_safe())
            unsafe_nbs.push_back(nbs[i]);
    }

    return unsafe_nbs;
}

void KnowledgeBase::check_add_safe(const Position &pos) {
    int row = pos.row;
    int col = pos.col;
    
    if(!m_map[row][col]->is_breezy()&&!m_map[row][col]->is_smelly()) {
        auto nbs = util::neighbors(m_row_len, m_col_len, Position(row,col));
        for(int i = 0; i < nbs.size(); i++){
            m_map[nbs[i].row][nbs[i].col]->add_state(Tile::TS_SAFE);
            m_map[nbs[i].row][nbs[i].col]->remove_state(Tile::TS_PIT);
            m_map[nbs[i].row][nbs[i].col]->remove_state(Tile::TS_WUMPUS);
        }
    }
}

void KnowledgeBase::infer_pit_wumpus(const Position &pos) {
    auto row = pos.row;
    auto col = pos.col;

    std::pair<Tile::TileState, Tile::TileState> target_states;
    if (m_map[row][col]->is_breezy())
        target_states.push_back(std::make_pair(Tile::TS_BREEZY, Tile::TS_PIT));
    if (m_map[row][col]->is_smelly())
        target_states.push_back(std::make_pair(Tile::TS_SMELLY, Tile::TS_WUMPUS));

    for (int i = 0; i < target_states.size(); i++) {
        auto state = target_states[i].second;
        auto nbs = util::neighbors(m_row_len, m_col_len, Position(row,col));
        auto vec_unsafe = unsafe_neighbors(nbs);
        if (vec_unsafe.size() == 1) {
            auto p = vec_unsafe[0];
            m_map[p.row][p.col]->set_state(state);
            m_map[p.row][p.col]->add_state(Tile::TS_DETERMINED);
        }
    }
}

void KnowledgeBase::infer_history(const Position &pos) {
    for (int i = m_history_pos.size() - 1; i >= 0; i--) {
        auto p = m_history_pos[i];
        infer_pit_wumpus(p);
    }
}

void KnowledgeBase::update_knowledge() {

}





