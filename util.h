/**
 * @author Jing Qin <qinjin@bu.edu>
 */

#ifndef UTIL_H
#define UTIL_H

#include "Board.h"
#include "Tile.h"
#include <unordered_map>
#include <string>

class KnowledgeBase;
namespace util {
typedef std::unordered_map<Position, Position> PathMap;
typedef std::unordered_map<Position, int> CostMap;
typedef std::unordered_map<Position, int> DegreeMap;

void display_board(const Board::Map &map);

/**
 * @brief a new map is an empty map which (0,0) is the entrance. 
 */ 
Board::Map create_new_map(int rows, int cols);
void destroy_map(Board::Map &map);

Board::Map read_map(const std::string &map_file_path);
Tile::TileState get_tile_state_by_code(const std::string &code);

PositionList neighbors(int rows, int cols, const Position &pos);
/**
 * @brief see if pos2 is a neighbor of pos1.
 */
bool is_neighbor(int rows, int cols, const Position &pos1, const Position &pos2);

/**
 * @brief calculate the minimum cost between pos1 and pos2
 * @return return the cost.
 */
int min_cost(const KnowledgeBase &kb, const Position &pos_from, const Position &pos_to, int degree_from, PositionList &move_path);
/**
 * @brief perform A* algorithm to get the mininum cost path.
 */
std::pair<PositionList, CostMap>
calc_best_path(const KnowledgeBase& kb, const Position& pos_from, const Position& pos_to, int degree_from);
/**
 * @brief calculate the cost between pos1 and pos2 which should be neighbors
 * @return return the cost if they are neighbors, otherwise return -1.
 */
int move_cost(int rows, int cols, const Position &pos1, const Position &pos2);
int rotate_cost(int curr_degree, const Position &pos_from, const Position &pos_to);
int heuristic(const Position &pos1, const Position &pos2);

int degree_by_direction(MoveDirection md);
MoveDirection direction_by_degree(int degree);

MoveDirection next_direction(const Position &pos_from, const Position &pos_to);

PositionList cross_positions(int rows, int cols, const Position &pos);

bool position_in(const Position &pos, const PositionList &pos_list);

void erase_position(const Position &pos, PositionList &pos_list);

std::string death_reason(Tile tile);
}

#endif // UTIL_H