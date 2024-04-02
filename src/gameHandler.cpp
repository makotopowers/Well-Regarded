#include <iostream>
#include <string>

#include "gameHandler.hpp"
#include "player.hpp"

GameHandler::GameHandler(Player *player1, Player *player2)
{
}

GameHandler::~GameHandler()
{
    std::cout << "GameHandler destructor called" << std::endl;
}