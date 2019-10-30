#include "KnowledgeBase.h"
#include "Tile.h"
#include "common.h"
#include "util.h"
#include <utility>
#include <iostream>

using namespace std;

KnowledgeBase::KnowledgeBase(int row_len, int col_len)
             :m_row_len(row_len)
             ,m_col_len(col_len)
{ 
    create_knowledge(row_len,col_len);   
}

KnowledgeBase::~KnowledgeBase(){

}

bool KnowledgeBase::is_safe(const Position &pos) const {
    return m_map[pos.row][pos.col]->is_safe();
}

PositionList KnowledgeBase::safe_neighbors(const Position &pos, bool use_history) const {
    PositionList safe_nbs;
    auto nbs = util::neighbors(m_row_len, m_col_len, pos);
    for(int i = 0; i < nbs.size(); i++) {
        if (is_safe(nbs[i]) && (use_history || !in_history(nbs[i])))
            safe_nbs.push_back(nbs[i]);
    }

    return safe_nbs;
}

PositionList KnowledgeBase::safe_history_neighbors(const Position &pos) const {
    PositionList safe_his_nbs;
    for (int i = m_history_pos.size() - 1; i >= 0 ; i--) {
        auto safe_nbs = safe_neighbors(m_history_pos[i]);
        for (int j = 0; j < safe_nbs.size(); j++) {
            safe_his_nbs.push_back(safe_nbs[j]);
        }
    }

    return safe_his_nbs;
}

PositionList KnowledgeBase::available_neighbors(const Position &pos, bool use_history) const {
    auto nbs = util::neighbors(m_row_len, m_col_len, pos);
    for (auto it = nbs.begin(); it != nbs.end();) {
        auto t = tile(*it);
        if (t->mustbe_wumpus() ||
            t->mustbe_pit() ||
            (!use_history && in_history(*it))) {
            it = nbs.erase(it);
        } else {
            ++it;
        }
    }
    return nbs;
}

PositionList KnowledgeBase::available_history_neighbors(const Position &pos) const {
    PositionList avail_his_nbs;
    for (int i = m_history_pos.size() - 1; i >= 0 ; i--) {
        auto avail_nbs = available_neighbors(m_history_pos[i]);
        for (int j = 0; j < avail_nbs.size(); j++) {
            avail_his_nbs.push_back(avail_nbs[j]);
        }
    }

    return avail_his_nbs;
}

void KnowledgeBase::on_wumpus_killed() {
    std::cout << __FUNCTION__ << std::endl;
    
    m_wumpus_killed = true;
    m_pos_wumpus = Position(-1, -1);

    clear_wumpus_smelly();
}

void KnowledgeBase::on_kill_wumpus_failed(const Position &pos, MoveDirection md) {
    switch (md) {
        case MD_UNKNOWN:
            return;
        case MD_NORTH: {
            for (int i = pos.row; i >= 0; i--) {
                m_map[i][pos.col]->remove_state(Tile::TS_WUMPUS);
                m_map[i][pos.col]->add_determined(Tile::TS_WUMPUS);
            }
            return;
        }
        case MD_SOUTH: {
            for (int i = pos.row; i < m_row_len; i++) {
                m_map[i][pos.col]->remove_state(Tile::TS_WUMPUS);
                m_map[i][pos.col]->add_determined(Tile::TS_WUMPUS);
            }
            return;
        }
        case MD_WEST: {
            for (int i = pos.col; i >= 0; i--) {
                m_map[pos.row][i]->remove_state(Tile::TS_WUMPUS);
                m_map[i][pos.col]->add_determined(Tile::TS_WUMPUS);
            }
            return;
        }
        case MD_EAST: {
            for (int i = pos.col; i < m_col_len; i++) {
                m_map[pos.row][i]->remove_state(Tile::TS_WUMPUS);
                m_map[i][pos.col]->add_determined(Tile::TS_WUMPUS);
            }
            return;
        }
        default:
            return;
    }
}

bool KnowledgeBase::wumpus_found() {
    return m_pos_wumpus != Position(-1, -1);
}

PositionList KnowledgeBase::possible_wumpus_positions() {
    return m_possible_wumpus_positions;
}
  
void KnowledgeBase::create_knowledge(int rows, int cols){ // Initialize the knowledge board
	m_map = util::create_new_map(rows, rows);
}

void KnowledgeBase::add_knowledge(const Position &pos, Tile status){ // Receive the new information and add to current map
    //if there are pit, wumpus, gold, the game is over. Therefore in this case, the program will not call this func.
    //So we just need to think about breezy and smelly.

    auto tile = m_map[pos.row][pos.col];
    if(status.is_entrance()){
        tile->add_state(Tile::TS_ENTRANCE);
    }

    using namespace std;
    vector< pair<Tile::TileState, Tile::TileState> > target_states;

    if (status.is_breezy())
        target_states.push_back(make_pair(Tile::TS_BREEZY, Tile::TS_PIT));
    if (status.is_smelly() && !m_wumpus_killed)
        target_states.push_back(make_pair(Tile::TS_SMELLY, Tile::TS_WUMPUS));
    else if (m_wumpus_killed)
        tile->remove_state(Tile::TS_SMELLY);
    
    for (int i = 0; i < target_states.size(); i++) {
        tile->add_state(target_states[i].first);
        auto nbs = util::neighbors(m_row_len, m_col_len, pos);
        for (int j = 0; j < nbs.size(); j++) {
            auto t = m_map[nbs[j].row][nbs[j].col];
            auto s = target_states[i].second;
            if(!t->is_safe()) {
                t->add_state(s);
                if (s == Tile::TS_WUMPUS && 
                    t->maybe_wumpus() && 
                    !util::position_in(nbs[j], m_possible_wumpus_positions)) {
                    m_possible_wumpus_positions.push_back(nbs[j]);
                }
                else if (s == Tile::TS_WUMPUS && t->mustnot_wumpus())
                    t->remove_state(s);
            }
        }
    }

    tile->remove_state(Tile::TS_PIT);
    tile->remove_state(Tile::TS_WUMPUS);
    tile->add_state(Tile::TS_SAFE);
    util::erase_position(pos, m_possible_wumpus_positions);

    add_knowledge_into_history(pos);

    analyze_knowledge(pos);
};

void KnowledgeBase::add_knowledge_into_history(const Position &pos) {
    for (int i = 0; i < m_history_pos.size(); i++)
        if (m_history_pos[i] == pos) return;

    m_history_pos.push_back(pos);
}

void KnowledgeBase::analyze_knowledge(const Position &pos) {//use the current information to infer the known space
    //rule: if 3 of 4 points around the position which is smelly or breezy is safe, the remaining point is the pit/wumpus.
    //search rule: searching from current position to the first position by using array m_row and m_pos_y.
    
    check_add_safe(pos);

    infer_pit_wumpus(pos);

    infer_history(pos);
}

PositionList KnowledgeBase::unsafe_neighbors(const PositionList &nbs) {
    PositionList unsafe_nbs;
    for (int i = 0; i < nbs.size(); i++) {
        if (!m_map[nbs[i].row][nbs[i].col]->is_safe())
            unsafe_nbs.push_back(nbs[i]);
    }

    return unsafe_nbs;
}

void KnowledgeBase::check_add_safe(const Position &pos) {
    auto tile = m_map[pos.row][pos.col];
    
    if(!tile->is_breezy() && !tile->is_smelly()) {
        auto nbs = util::neighbors(m_row_len, m_col_len, pos);
        for(int i = 0; i < nbs.size(); i++) {
            m_map[nbs[i].row][nbs[i].col]->add_state(Tile::TS_SAFE);
            m_map[nbs[i].row][nbs[i].col]->remove_state(Tile::TS_PIT);
            m_map[nbs[i].row][nbs[i].col]->remove_state(Tile::TS_WUMPUS);
            util::erase_position(nbs[i], m_possible_wumpus_positions);
        }
    }
}

void KnowledgeBase::infer_pit_wumpus(const Position &pos) {
    auto tile = m_map[pos.row][pos.col];

    std::vector< std::pair<Tile::TileState, Tile::TileState> > target_states;
    if (tile->is_breezy())
        target_states.push_back(std::make_pair(Tile::TS_BREEZY, Tile::TS_PIT));
    if (tile->is_smelly() && !m_wumpus_killed)
        target_states.push_back(std::make_pair(Tile::TS_SMELLY, Tile::TS_WUMPUS));
    else if (m_wumpus_killed)
        tile->remove_state(Tile::TS_SMELLY);

    auto nbs = util::neighbors(m_row_len, m_col_len, pos);
    auto vec_unsafe = unsafe_neighbors(nbs);
    for (int i = 0; i < target_states.size(); i++) {
        auto state = target_states[i].second;
        if (vec_unsafe.size() == 1) {
            auto p = vec_unsafe[0];
            m_map[p.row][p.col]->set_state(state);
            m_map[p.row][p.col]->add_determined(state);
            
            check_found_wumpus(p);
        }
    }
}

void KnowledgeBase::check_found_wumpus(const Position &pos) {
    auto t = tile(pos);
    if (!t->mustbe_wumpus()) return;

    m_pos_wumpus = pos;

    for (int i = 0; i < m_row_len; i++) {
        for (int j = 0; j < m_col_len; j++) {
            if (m_map[i][j]->maybe_wumpus()) {
                m_map[i][j]->remove_state(Tile::TS_WUMPUS);
                m_map[i][j]->add_determined(Tile::TS_WUMPUS);
            }
        }
    }

    for (auto it = m_possible_wumpus_positions.begin(); it != m_possible_wumpus_positions.end();) {
        if (*it != pos)
            it = it = m_possible_wumpus_positions.erase(it);
        else
            ++it;
    }
}

void KnowledgeBase::infer_history(const Position &pos) {
    for (int i = m_history_pos.size() - 1; i >= 0; i--) {
        auto p = m_history_pos[i];
        infer_pit_wumpus(p);
    }
}

void KnowledgeBase::clear_wumpus_smelly() {
    for (int i = 0; i < m_map.size(); i++) {
        for (int j = 0; j < m_map[i].size(); j++) {
            m_map[i][j]->remove_state(Tile::TS_WUMPUS);
            m_map[i][j]->remove_state(Tile::TS_SMELLY);
        }
    }
}

std::shared_ptr<Tile> KnowledgeBase::tile(const Position &pos) const {
    return m_map[pos.row][pos.col];
}

bool KnowledgeBase::in_history(const Position &pos) const {
    for (int i = 0; i < m_history_pos.size(); i++) {
        if (pos == m_history_pos[i])
            return true;
    }

    return false;
}