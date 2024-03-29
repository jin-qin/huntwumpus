/**
 * @author Jing Qin <qinjin@bu.edu>
 */

#ifndef TILE_H
#define TILE_H

class Tile {
public:
  enum TileState {
    TS_UNKNOWN              = 0x0000,
    TS_ENTRANCE             = 0x0001,
    TS_BREEZY               = 0x0002,
    TS_SMELLY               = 0x0004,
    TS_WUMPUS               = 0x0008,     // May be a wumpus if the state is not DETERMINED.
    TS_GOLD                 = 0x0010,     // May be gold if the state is not DETERMINED.
    TS_PIT                  = 0x0020,     // May be a pit if the state is not DETERMINED.
    TS_PLAYER_HERE          = 0x0040,
    TS_PIT_DETERMINED       = 0x0080,
    TS_WUMPUS_DETERMINED    = 0x0100,
    TS_SAFE                 = 0x0200,     //Shuo added this var to represent the safe status
  };

  Tile();
  Tile(TileState ts);
  Tile(int ts);

  void set_state(TileState ts);
  void set_state(int ts);
  void add_state(TileState ts);
  void add_state(int ts);
  void remove_state(TileState ts);
  void remove_state(int ts);
  TileState state();

  void add_determined(TileState ts);

  void update_state();

  bool is_entrance();
  bool is_breezy();
  bool is_smelly();
  bool has_wumpus();
  bool maybe_wumpus();
  bool mustbe_wumpus();
  bool mustnot_wumpus();
  bool has_pit();
  bool maybe_pit();
  bool mustbe_pit();
  bool mustnot_pit();
  bool has_gold();
  bool player_been_here();
  bool is_safe();

private:
  bool wumpus_determined();
  bool pit_determined();

  TileState m_state = TS_UNKNOWN;
};

#endif // TILE_H