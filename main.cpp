

#include <iostream>
#include <string>
#include "player.hpp"
#include "gameHandler.hpp"

int main()
{
    std::cout << "Hello, World!" << std::endl;

    Player *player1 = new Player();
    Player *player2 = new Player();
    GameHandler gameHandler(player1, player2);

    delete player1;
    delete player2;

    return 0;
}
