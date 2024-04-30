#ifndef EXPERIMENTS_HPP
#define EXPERIMENTS_HPP

#include <iostream>
#include <string>

#include "player.hpp"
#include "utilities.hpp"

class Experiments {
 public:
  static const int DECK_SIZE = 53;
  static const int TIE = 0;
  static const int PLAYER1_AHEAD = 1;
  static const int PLAYER2_AHEAD = 2;
  Experiments();
  ~Experiments();

  std::vector<int> spotTest(
      std::shared_ptr<Player> player1, std::shared_ptr<Player> player2,
      int iterations = 1000);  // see how many cards it takes
                               // for hand1 to beat hand2

 private:
  void Log(std::string message, bool debug = true);
  int mean(std::vector<int> vec);
  int variance(std::vector<int> vec);
};

#endif  // EXPERIMENTS_HPP