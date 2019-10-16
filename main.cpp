//
//  main.cpp
//  HuntTheWumpus
//
//  Created by Jonathan Redwine on 10/2/19.
//  Copyright © 2019 JonathanRedwine. All rights reserved.
//

#include <iostream>
#include "Board.h"
#include "Player.h"
using namespace std;

int main(int argc, const char * argv[]) {
    Board board;
    board.display_board();
    Player player;
    cout << player.currPosX << " " << player.currPosY << endl;
    
    cout << player.select_move() << endl;
    
    cout << player.currPosX << " " << player.currPosY << endl;
    

    return 0;
}
