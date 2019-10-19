/**
 * @author Jing Qin <qinjin@bu.edu>
 */

#ifndef DEFINES_H
#define DEFINES_H

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

#endif