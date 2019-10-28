#include "Tile.h"

Tile::Tile()
    : m_state(TS_UNKNOWN) {
}

Tile::Tile(TileState ts)
    : m_state(ts) {
}

Tile::Tile(int ts)
    : m_state(static_cast<TileState>(ts)) {
}

void Tile::set_state(TileState ts) {
    m_state = ts;
}

void Tile::set_state(int ts) {
    set_state(static_cast<TileState>(ts));
}

void Tile::add_state(TileState ts) {
    set_state(m_state | ts);
}
void Tile::add_state(int ts) {
    set_state(m_state | static_cast<TileState>(ts));
}
void Tile::remove_state(TileState ts) {
    set_state(m_state & ~ts);
}
void Tile::remove_state(int ts) {
    set_state(m_state & static_cast<TileState>(~ts));
}

Tile::TileState Tile::state() {
    return m_state;
}

void Tile::add_determined(TileState ts) {
    if (ts == TS_PIT)
        add_state(TS_PIT_DETERMINED);
    if (ts == TS_WUMPUS)
        add_state(TS_WUMPUS_DETERMINED);
}

void Tile::update_state() {
    // TO DO
}

bool Tile::is_entrance() {
    return 0 != (m_state & TS_ENTRANCE);
}

bool Tile::is_breezy() {
    return 0 != (m_state & TS_BREEZY);
}
bool Tile::is_smelly() {
    return 0 != (m_state & TS_SMELLY);
}
bool Tile::has_wumpus() {
    return 0 != (m_state & TS_WUMPUS);
}
bool Tile::maybe_wumpus() {
    return has_wumpus() && !wumpus_determined();
}
bool Tile::mustbe_wumpus() {
    return has_wumpus() && wumpus_determined();
}
bool Tile::mustnot_wumpus() {
    return !has_wumpus() && wumpus_determined();
}
bool Tile::has_pit() {
    return 0 != (m_state & TS_PIT);
}
bool Tile::maybe_pit() {
    return has_pit() && !pit_determined();
}
bool Tile::mustbe_pit() {
    return has_pit() && pit_determined();
}
bool Tile::mustnot_pit() {
    return !has_pit() && pit_determined();
}
bool Tile::has_gold() {
    return 0 != (m_state & TS_GOLD);
}
bool Tile::player_been_here() {
    return 0 != (m_state & TS_PLAYER_HERE);
}

bool Tile::wumpus_determined() {
    return 0 != (m_state & TS_WUMPUS_DETERMINED);
}
bool Tile::pit_determined() {
    return 0 != (m_state & TS_PIT_DETERMINED);
}

bool Tile::is_safe() {
    return 0 != (m_state & TS_SAFE);
}