/**
 * @author Jing Qin <qinjin@bu.edu>
 */

#ifndef DEFINES_H
#define DEFINES_H

#include <functional>

template<typename T> void safe_delete(T*& a) {
  if (!a) return;
  delete a;
  a = 0;
}

enum MoveDirection {
    MD_UNKNOWN = 0,
    MD_NORTH,
    MD_EAST,
    MD_SOUTH,
    MD_WEST
};

class Position{
public:
  Position(): row(0), col(0), priority(0) {}
  Position(const Position &pos): row(pos.row), col(pos.col), priority(pos.priority) {}
  Position(int r, int c):row(r),col(c),priority(0) {}
  Position(int r, int c, int p):row(r),col(c),priority(p) {}

  void operator = (const Position &pos) {
    row = pos.row;
    col = pos.col;
    priority = pos.priority;
  }

  bool operator == (const Position &pos) const {
    return (pos.row == row && pos.col == col);
  }

  bool operator != (const Position &pos) const {
    return !((*this) == pos);
  }

  bool operator < (const Position &pos) const {
    return priority < pos.priority;
  }
  
  int row = 0;
  int col = 0;
  int priority = 0;
};

namespace std {
    template<> struct hash<Position> {
        std::size_t operator()(Position const& pos) const noexcept {
            std::size_t h1 = std::hash<int>()(pos.row);
            std::size_t h2 = std::hash<int>()(pos.col);
            std::size_t h3 = std::hash<int>()(pos.priority);
            return (h1 ^ (h2 << 1) >> 1) ^ (h3 << 1);
        }
    };
}

typedef std::vector< Position > NeighborsList;

#endif