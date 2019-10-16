#include "../Tile.h"

Tile test_map[5][5] =
    {{Tile(Tile::TS_ENTRANCE), //0,0
        Tile(Tile::TS_BREEZY), //0,1
        Tile(Tile::TS_PIT), //0,2
        Tile(Tile::TS_BREEZY), //0,3
        Tile(Tile::TS_UNKNOWN)}, //0,4
        {Tile(Tile::TS_UNKNOWN), //1,0
            Tile(Tile::TS_UNKNOWN), //1,1
            Tile(Tile::TS_BREEZY), //1,2
            Tile(Tile::TS_UNKNOWN), //1,3
            Tile(Tile::TS_UNKNOWN)}, //1,4
        {Tile(Tile::TS_BREEZY), //2,0
            Tile(Tile::TS_UNKNOWN), //2,1
            Tile(Tile::TS_SMELLY), //2,2
            Tile(Tile::TS_UNKNOWN), //2,3
            Tile(Tile::TS_BREEZY)}, //2,4
        {Tile(Tile::TS_PIT), //3,0
            Tile(Tile::TS_BREEZY | Tile::TS_SMELLY), //3,1
            Tile(Tile::TS_WUMPUS), //3,2
            Tile(Tile::TS_BREEZY | Tile::TS_SMELLY | Tile::TS_GOLD), //3,3
            Tile(Tile::TS_GOLD)}, //3,4
        {Tile(Tile::TS_BREEZY), //4,0
            Tile(Tile::TS_PIT), //4,1
            Tile(Tile::TS_BREEZY | Tile::TS_SMELLY), //4,2
            Tile(Tile::TS_UNKNOWN), //4,3
            Tile(Tile::TS_BREEZY)} //4,4
    };

int main()
{
    return 0;
}