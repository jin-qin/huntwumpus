#include "Tile.h"

Tile::Tile()
    : m_state(TS_UNKNOWN)
{
}

Tile::Tile(TileState ts)
    : m_state(ts)
{
}

Tile::Tile(int ts)
    : m_state(static_cast<TileState>(ts))
{
}

void Tile::set_state(TileState ts)
{
    m_state = ts;
}

void Tile::set_state(int ts)
{
    set_state(static_cast<TileState>(ts));
}

void Tile::add_state(TileState ts)
{
    set_state(m_state | ts);
}
void Tile::add_state(int ts)
{
    set_state(m_state | static_cast<TileState>(ts));
}
void Tile::remove_state(TileState ts)
{
    set_state(m_state & ~ts);
}
void Tile::remove_state(int ts)
{
    set_state(m_state & static_cast<TileState>(~ts));
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
bool Tile::maybe_wumpus()
{
    return has_wumpus() && !determined();
}
bool Tile::mustbe_wumpus()
{
    return has_wumpus() && determined();
}
bool Tile::has_pit()
{
    return 0 != (m_state & TS_PIT);
}
bool Tile::maybe_pit()
{
    return has_pit() && !determined();
}
bool Tile::mustbe_pit()
{
    return has_pit() && determined();
}
bool Tile::has_gold()
{
    return 0 != (m_state & TS_GOLD);
}
bool Tile::player_been_here()
{
    return 0 != (m_state & TS_PLAYER_HERE);
}
bool Tile::determined()
{
    return 0 != (m_state & TS_DETERMINED);
}