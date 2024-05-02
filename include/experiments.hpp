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

  std::vector<double> spotTestp(std::unique_ptr<Player>& player1, std::unique_ptr<Player>& player2, int iterations = 1000,
                                bool visual = false);  // see how many cards it takes
                                                       // for hand1 to beat hand2
  void spotTest(std::unique_ptr<Player>& player1, std::unique_ptr<Player>& player2, int iterations, bool visual, std::vector<int>& results,
                std::vector<int>& visualResults, std::mutex& mtx, std::mutex& visualMtx);

 private:
  double mean(std::vector<int> vec);
  double variance(std::vector<int> vec);
};

#endif  // EXPERIMENTS_HPP