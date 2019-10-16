#include "Tile.h"

Tile::Tile(TileState ts)
    : m_state(ts)
{
}

Tile::Tile(int ts)
    : m_state(static_cast<Tile::TileState>(ts))
{
}

void Tile::set_state(Tile::TileState ts)
{
    m_state = ts;
}

void Tile::set_state(int ts)
{
    set_state(static_cast<Tile::TileState>(ts));
}

Tile::TileState Tile::state()
{
    return m_state;
}

void Tile::update_state()
{
    // TO DO
}

bool Tile::is_entrance()
{
    return 0 != (m_state & TS_ENTRANCE);
}

bool Tile::is_breezy()
{
    return 0 != (m_state & TS_BREEZY);
}
bool Tile::is_smelly()
{
    return 0 != (m_state & TS_SMELLY);
}
bool Tile::has_wumpus()
{
    return 0 != (m_state & TS_WUMPUS);
}
bool Tile::has_gold()
{
    return 0 != (m_state & TS_GOLD);
}
bool Tile::has_pit()
{
    return 0 != (m_state & TS_PIT);
}
bool Tile::player_been_here()
{
    return 0 != (m_state & TS_PLAYER_HERE);
}
bool Tile::determined()
{
    return 0 != (m_state & TS_DETERMINED);
}