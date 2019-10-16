/**
 * @author Jing Qin <qinjin@bu.edu>
 */

#ifndef DEFINES_H
#define DEFINES_H

template<typename T> void safe_delete(T*& a) {
  delete a;
  a = 0;
}

#endif