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
using namespace std;

int main(int argc, const char * argv[]) {
    auto board = make_shared<Board>();
    board->generate_new_map(5, 5);
    board->display_board();
    Player player(board);
    cout << player.curr_pos().row << " " << player.curr_pos().col << endl;

    while (!player.game_over()) {
        auto result = player.select_move();
        if (result.empty()) {
            break;
        }

        cout << player.curr_pos().row << " " << player.curr_pos().col << endl;
    }

    cout << player.score() << endl;

    return 0;
}
