#include "util.h"
#include "Tile.h"
#include "Board.h"
#include "KnowledgeBase.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <algorithm>
#include <functional>
#include "json/json.h"

namespace util {

void display_board(const Board::Map &map) {   
    std::cout << __FUNCTION__ << ":: " << "####### MAP START #######" << std::endl;
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
    std::cout << __FUNCTION__ << ":: " << "######## MAP END ########" << std::endl;
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

Board::Map read_map(const std::string& map_file_path) {
	std::ifstream ifs(map_file_path);
	if (!ifs.is_open()) {
		std::cout << __FUNCTION__ << ":: cannot find the map file! pls check your map file path!" << std::endl;
		return Board::Map();
	}

	std::ostringstream tmp;
	tmp << ifs.rdbuf();
	std::string str_json = tmp.str();

	Json::Reader json_reader;
	Json::Value root;
	if (!json_reader.parse(str_json, root)) {
		std::cout << __FUNCTION__ << ":: wrong map file (must be legal json format!)!" << std::endl;
		return Board::Map();
	}

	auto map_size = root["map_size"];
	auto map_data = root["map_data"];
	if (map_size.isNull() ||
		map_data.isNull() ||
		!map_data.isArray() ||
		map_size["rows"].isNull() ||
		map_size["cols"].isNull()) {

		std::cout << __FUNCTION__ << ":: wrong map parameters, pls check again!" << std::endl;
		return Board::Map();
	}

	auto rows = map_size["rows"].asInt();
	auto cols = map_size["cols"].asInt();
	auto new_map = create_new_map(rows, cols);
	for (auto item: map_data) {
		if (item["type"].isNull() || item["pos"].isNull() || 
			item["pos"]["row"].isNull() || item["pos"]["col"].isNull()) {
			std::cout << __FUNCTION__ << ":: wrong map parameters, pls check again!" << std::endl;
			return Board::Map();
		}
		auto row = item["pos"]["row"].asInt();
		auto col = item["pos"]["col"].asInt();
		auto ts = get_tile_state_by_code(item["type"].asString());
		new_map[row][col]->add_state(ts);
	}

	return new_map;
}

Tile::TileState get_tile_state_by_code(const std::string& code)
{
	if (code == "E")
		return Tile::TS_ENTRANCE;
	if (code == "P")
		return Tile::TS_PIT;
	if (code == "W")
		return Tile::TS_WUMPUS;
	if (code == "G")
		return Tile::TS_GOLD;
	return Tile::TileState();
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

    auto pos = came_from[goal];
    best_path.push_back(goal);
    while(came_from[pos] != Position(-1, -1)) {
        best_path.push_back(pos);
        pos = came_from[pos];
    }
    reverse(best_path.begin(), best_path.end());

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
            it = pos_list.erase(it);
        else
            ++it;
    }
}

std::string death_reason(Tile tile) {
    if (tile.mustbe_pit()) {
        return std::string("Oops! You are killed by a PIT!");
    }
    if (tile.mustbe_wumpus()) {
        return std::string("Oops! You are killed by the WUMPUS!");
    }

	return std::string("Wait, wait...You died for unknown reasons? Lol, are you killed by the air?");
}

}