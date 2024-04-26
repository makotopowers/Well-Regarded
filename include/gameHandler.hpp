#ifndef GAMEHANDLER_HPP
#define GAMEHANDLER_HPP

#include <iostream>
#include <string>
#include <vector>

#include "player.hpp"

class GameHandler {
 public:
  GameHandler(std::unique_ptr<Player> player1_,
              std::unique_ptr<Player> player2_);
  std::unique_ptr<Player> player1;
  std::unique_ptr<Player> player2;
  void startGame();
  void endGame();
  void printPlayers();
  void testFunc();
  ~GameHandler();

  int compareHand(std::vector<int> p1hand, std::vector<int> p2hand);

 private:
  std::vector<Player> players;
  void tick();
  void Log(std::string message, bool debug = true);
};

#endif  // GAMEHANDLER_HPP