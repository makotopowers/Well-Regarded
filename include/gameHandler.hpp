#ifndef GAMEHANDLER_HPP
#define GAMEHANDLER_HPP

#include <iostream>
#include <string>
#include <vector>

#include "player.hpp"

class GameHandler
{
public:
    GameHandler(Player *player1, Player *player2);
    void startGame();
    void endGame();
    void printPlayers();
    ~GameHandler();

private:
    std::vector<Player> players;
    void tick();
};

#endif // GAMEHANDLER_HPP