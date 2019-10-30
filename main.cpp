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
	} else {
		cout << "Wrong parameters number! Parameters should be exactly one, i.e. the path of the map file!" << endl;
		return -1;
	}

    board->display_board();
    Player player(board);
    player.set_game_mode(Player::GM_GET_GOLD | Player::GM_KILL_WUMPUS);

    while (!player.game_over()) {
        auto result = player.select_move();
        if (result.empty()) {
            break;
        }
    }

	cout << "##################### GAME OVER #####################" << endl;
	if (player.wumpus_killed()) {
		cout << "Great! You killed the WUMPUS!" << endl;
	}
	if (player.has_gold()) {
		cout << "Good job! You got the gold!" << endl;
	}

    cout << "Your score: " << player.score() << endl;
	cout << "##################### GAME OVER #####################" << endl;

    return 0;
}
