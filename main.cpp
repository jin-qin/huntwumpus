//
//  main.cpp
//  HuntTheWumpus
//
//  Created by Jonathan Redwine on 10/2/19.
//  Copyright © 2019 JonathanRedwine. All rights reserved.
//

#include <iostream>
#include <memory>
#include "Board.h"
#include "Player.h"
#include "util.h"
using namespace std;

int main(int argc, const char * argv[]) {
	if (argc != 2) {
		cout << "Wrong parameters number! Parameters should be exactly one, i.e. the path of the map file!" << endl;
		return -1;
	}

	auto board = make_shared<Board>();
	if (argc == 2) {
		std::string map_path = argv[1];
		auto map = util::read_map(map_path);
		if (map.empty()) {
			return -1;
		}
		board->set_map(map);
	} else if (argc == 1) {
		board->generate_new_map(5, 5);
	}

    board->display_board();
    Player player(board);
    player.set_game_mode((Player::GameMode)(Player::GM_GET_GOLD | Player::GM_KILL_WUMPUS));

    while (!player.game_over()) {
        auto result = player.select_move();
        if (result.empty()) {
            break;
        }
    }

    cout << "game over, score: " << player.score() << endl;

    return 0;
}
