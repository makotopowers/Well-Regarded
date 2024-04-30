#include "utilities.hpp"

#include <iostream>

#include "player.hpp"

Utilities::Utilities() {}
Utilities::~Utilities() {}

void Utilities::Log(std::string message, bool debug) {
  if (debug) {
    std::cout << message << std::endl;
  }
}

int Utilities::compareHand(std::vector<int> p1hand, std::vector<int> p2hand) {
  /// @brief Compare the hands of two players
  /// @param p1hand std::vector<int> representing the hand of player 1
  /// @param p2hand std::vector<int> representing the hand of player 2
  /// @return 1 if player 1 is ahead, 2 if player 2 is ahead, 0 if tie

  for (int i = 0; i < p1hand.size(); i++) {
    if (p1hand[i] > p2hand[i]) {
      return 1;
    } else if (p1hand[i] < p2hand[i]) {
      return 2;
    } else {
      continue;
    }
  }
  return 0;
}
