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
    board->generate_new_map(10, 10);
    board->display_board();
    Player player(board);
    cout << player.curr_pos_row() << " " << player.curr_pos_col() << endl;
    
    cout << player.select_move() << endl;
    
    cout << player.curr_pos_row() << " " << player.curr_pos_col() << endl;
    

    return 0;
}
