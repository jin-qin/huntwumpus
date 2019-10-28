#include "util.h"
#include "Tile.h"
#include "Board.h"
#include "KnowledgeBase.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <functional>

namespace util {

void display_board(const Board::Map &map) {   
    std::cout << __FUNCTION__ << ":: " << "####### START #######" << std::endl;
    for (size_t i = 0; i < map.size(); i++) {
        for (size_t j = 0; j < map[i].size(); j++) {
            int empty = 1;
            std::cout << "  " << i << "," << j << ": ";
            if (map[i][j]->is_entrance()) {
                empty = 0;
                std::cout << "E";
            }
            if (map[i][j]->is_breezy()) {
                empty = 0;
                std::cout << "B";
            }
            if (map[i][j]->is_smelly()) {
                empty = 0;
                std::cout << "S";
            }
            if (map[i][j]->has_wumpus()) {
                empty = 0;
                std::cout << "W";
            }
            if (map[i][j]->has_gold()) {
                empty = 0;
                std::cout << "G";
            }
            if (map[i][j]->has_pit()) {
                empty = 0;
                std::cout << "P";
            }
            if (empty) {
                std::cout << "N";
            }
        }
        std::cout << std::endl;
    }
    std::cout << __FUNCTION__ << ":: " << "######## END ########" << std::endl;
}

Board::Map create_new_map(int rows, int cols) {
    if (rows <= 0 || cols <= 0) {
        std::cout << __FUNCTION__ << "::wrong rows or cols specified!" << std::endl;
        return Board::Map();
    }

    Board::Map map(rows, std::vector<std::shared_ptr<Tile>>());
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            map[i].push_back(std::make_shared<Tile>());

    if (!map[0][0]) {
        std::cout << __FUNCTION__ << "::fatal error, empty pointer of the map, this should not appear!" << std::endl;
        return Board::Map();
    }
    map[0][0]->set_state(Tile::TS_ENTRANCE);

    return map;
}

PositionList neighbors(int rows, int cols, const Position &pos) {
    auto row = pos.row;
    auto col = pos.col;
    auto nbs = PositionList();

    if (col > 0)
        nbs.push_back(Position(row, col - 1));
    if (row > 0)
        nbs.push_back(Position(row - 1, col));
    if (col < cols - 1)
        nbs.push_back(Position(row, col + 1));
    if (row < rows - 1)
        nbs.push_back(Position(row + 1, col));
    return nbs;
}

bool is_neighbor(int rows, int cols, const Position &pos1, const Position &pos2) {
    auto nbs = neighbors(rows, cols, pos1);
    for (int i = 0; i < nbs.size(); i++) {
        if (pos2 == nbs[i])
            return true;
    }
    
    return false;
}

int min_cost(const KnowledgeBase &kb,
            const Position &pos_from, const Position &pos_to, int degree_from,
            PositionList &move_path) {

    auto rows = kb.rows();
    auto cols = kb.cols();

    bool is_nb = is_neighbor(rows, cols, pos_from, pos_to);
    if (is_nb) {
        move_path = PositionList({pos_to});
        return rotate_cost(degree_from, pos_from, pos_to);
    }

    auto result = calc_best_path(kb, pos_from, pos_to, degree_from);
    auto cost_map = result.second;
    move_path = result.first;
    return cost_map[pos_to];
}

std::pair<PositionList, CostMap>
calc_best_path(const KnowledgeBase &kb,
                const Position &pos_from, const Position &pos_to, int degree_from) {
    using namespace std;
    PositionList best_path;
    
    int rows = kb.rows();
    int cols = kb.cols();

    // perform A*
    priority_queue<Position, PositionList> frontier;
    frontier.push(pos_from);

    auto goal = pos_to; // goal is the entrance.
    PathMap came_from;
    CostMap cost_so_far;
    DegreeMap degrees;
    came_from[pos_from] = Position(-1, -1);
    cost_so_far[pos_from] = 0;
    degrees[pos_from] = degree_from;

    // std::cout << __FUNCTION__ << ":: goal: " << goal.row << goal.col << std::endl;
    while (!frontier.empty()) {
        auto current = frontier.top();
        frontier.pop();

        if (goal == current)
            break;
        
        auto nbs = kb.safe_neighbors(current, true);
        auto avail_nbs = kb.available_neighbors(current, false);
        for (int i = 0; i < avail_nbs.size(); i++) {
            if (avail_nbs[i] == goal)
                nbs.push_back(goal);
        }
        for (int i = 0; i < nbs.size(); i++) {
            auto nb = nbs[i];
            
            auto new_cost = cost_so_far[current] + move_cost(rows, cols, current, nb) + \
                            rotate_cost(degrees[current], current, nb);

            if (cost_so_far.find(nb) == cost_so_far.end() ||
                new_cost < cost_so_far[nb]) {
                cost_so_far[nb] = new_cost;
                nb.priority = new_cost + heuristic(goal, nb);
                frontier.push(nb);
                degrees[nb] = degree_by_direction(next_direction(current, nb));
                came_from[nb] = current;
            }
        }
    }
    
    // std::cout << __FUNCTION__ << ":: ### path map" << std::endl;
    // for (auto it = came_from.begin(); it != came_from.end(); ++it) {
    //     std::cout << __FUNCTION__ << ":: " << it->first.row << it->first.col << ";" \
    //               << it->second.row << it->second.col << std::endl;
    // }
    // std::cout << __FUNCTION__ << ":: ############" << std::endl;
    // std::cout << __FUNCTION__ << ":: ### cost map" << std::endl;
    // for (auto it = cost_so_far.begin(); it != cost_so_far.end(); ++it) {
    //     std::cout << __FUNCTION__ << ":: " << it->first.row << it->first.col << ";" \
    //               << it->second << std::endl;
    // }
    // std::cout << __FUNCTION__ << ":: ############" << std::endl;

    auto pos = came_from[goal];
    best_path.push_back(goal);
    // std::cout << __FUNCTION__ << ":: goal came from: " << pos.row << pos.col << std::endl;
    while(came_from[pos] != Position(-1, -1)) {
        // std::cout << __FUNCTION__ << ":: pos: " << pos.row << pos.col << std::endl;
        best_path.push_back(pos);
        pos = came_from[pos];
    }
    reverse(best_path.begin(), best_path.end());

    // std::cout << __FUNCTION__ << ":: best path: ";
    // for (int i = 0; i < best_path.size(); i++) {
    //     std::cout << best_path[i].row << best_path[i].col << "->";
    // }
    // std::cout << std::endl;

    return std::make_pair(best_path, cost_so_far);
}

int move_cost(int rows, int cols, const Position &pos1, const Position &pos2) {
    auto nbs1 = neighbors(rows, cols, pos1);
    for (int i = 0; i < nbs1.size(); i++)
        if (nbs1[i] == pos2)
            return 1;

    return -1;
}

int rotate_cost(int curr_degree, const Position &pos_from, const Position &pos_to) {
    auto md = next_direction(pos_from, pos_to);
    auto degree_rotate = abs((util::degree_by_direction(md) - curr_degree));
    degree_rotate = degree_rotate > 180 ? 360 - degree_rotate : degree_rotate;

    return degree_rotate / 90;
}

int heuristic(const Position &pos1, const Position &pos2) {
    return abs(pos1.row - pos2.row) + abs(pos1.col - pos2.col);
}

int degree_by_direction(MoveDirection md) {
	switch (md) {
	case MD_UNKNOWN:
		std::cout << __FUNCTION__ << ":: wrong direction, pls check your codes!" << std::endl;
		return 0;
	case MD_NORTH:
		return 0;
	case MD_EAST:
		return 90;
	case MD_SOUTH:
		return 180;
	case MD_WEST:
		return 270;
	default:
		std::cout << __FUNCTION__ << ":: wrong direction, pls check your codes!" << std::endl;
		return 0;
	}
}

MoveDirection direction_by_degree(int degree) {
    switch (degree) {
	case 0:
		return MD_NORTH;
	case 90:
		return MD_EAST;
	case 180:
		return MD_SOUTH;
	case 270:
		return MD_WEST;
	default:
		std::cout << __FUNCTION__ << ":: wrong direction, pls check your codes!" << std::endl;
		return MD_UNKNOWN;
	}
}

MoveDirection next_direction(const Position &pos_from, const Position &pos_to) {
    if ((abs(pos_to.row - pos_from.row) == 1 && abs(pos_to.col - pos_from.col) == 1)) {
        std::cout << __FUNCTION__ << ":: wrong parameters, pos_to: " \
        << pos_to.row << pos_to.col << " should not in the diags of pos_from: " \
        << pos_from.row << pos_from.col << std::endl;
        return MD_UNKNOWN;
    }

    if (pos_to.row - pos_from.row > 0)
        return MD_SOUTH;
    else if (pos_to.row - pos_from.row < 0)
        return MD_NORTH;

    if (pos_to.col - pos_from.col > 0)
        return MD_EAST;
    else if (pos_to.col - pos_from.col < 0)
        return MD_WEST;

    return MD_UNKNOWN;
}

PositionList cross_positions(int rows, int cols, const Position &pos) {
    PositionList corss_pos_list;
    for (int i = 0; i < cols; i++) {
        if (i == pos.col) continue;
        corss_pos_list.push_back(Position(pos.row, i));
    }
    for (int i = 0; i < rows; i++) {
        if (i == pos.row) continue;
        corss_pos_list.push_back(Position(i, pos.col));
    }
    return corss_pos_list;
}

bool position_in(const Position &pos, const PositionList &pos_list) {
    for (int i = 0; i < pos_list.size(); i++) {
        if (pos == pos_list[i])
            return true;
    }

    return false;
}

void erase_position(const Position &pos, PositionList &pos_list) {
    for (auto it = pos_list.begin(); it != pos_list.end();) {
        if (pos == *it)
            pos_list.erase(it);
        else
            ++it;
    }
}

}