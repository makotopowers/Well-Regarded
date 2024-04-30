
#include "experiments.hpp"

#include <fstream>
#include <vector>

#include "player.hpp"

Experiments::Experiments() {
  /// @brief Constructor for Experiments
  Utilities util;
  this->Log("Experiments constructor called");
}

Experiments::~Experiments() {
  /// @brief Destructor for Experiments
  this->Log("Experiments destructor called");
}

std::vector<int> Experiments::spotTest(std::shared_ptr<Player> player1,
                                       std::shared_ptr<Player> player2,
                                       int iterations) {
  /// @brief see how many cards it takes for hand1 to beat hand2
  /// @param hand1 std::shared_ptr<Hand> hand1
  /// @param hand2 std::shared_ptr<Hand> hand2
  int ahead = TIE;

  ahead = Utilities::compareHand(player1->hand->value, player2->hand->value);
  if (ahead == TIE) {
    this->Log("Tie");
    return std::vector<int>{0, 0};
  } else if (ahead == PLAYER2_AHEAD) {  // not sure if should do this
    return spotTest(player2, player1, iterations);
  }

  int beforeTricks = player2->tricks;
  std::cout << "beforeTricks: " << beforeTricks << std::endl;
  std::vector<int> beforePlayOrder = player2->playOrder;
  for (int i = 0; i < player2->playOrder.size(); i++) {
    std::cout << "beforePlayOrder[" << i << "]: " << beforePlayOrder[i]
              << std::endl;
  }

  std::vector<int> results;
  std::vector<int> visuals = std::vector<int>(54, 0);  // for debugging
  results.reserve(iterations);
  int count = 0;
  for (int _ = 0; _ < iterations; _++) {
    count = 0;
    ahead = Utilities::compareHand(player1->hand->value, player2->hand->value);

    while (ahead == PLAYER1_AHEAD) {
      player2->playCard();
      count += 1;
      player2->evaluateHand();
      ahead =
          Utilities::compareHand(player1->hand->value, player2->hand->value);
    }
    player2->printHand(player2->evaluateHand());
    results.push_back(count);
    visuals[count] += 1;

    std::cout << "iteration: " << _ << std::endl;
    std::cout << "--------------------------------" << std::endl;
    player2->setState(beforeTricks, beforePlayOrder);
    player2->evaluateHand();

    // ahead = Utilities::compareHand(player1->hand->value,
    // player2->hand->value);
  }

  std::ofstream myfile;
  myfile.open("results.txt");
  for (int i = 0; i < visuals.size(); i++) {
    myfile << visuals[i] << std::endl;
  }
  myfile.close();

  std::vector<int> returnVec;
  returnVec.push_back(mean(results));
  returnVec.push_back(variance(results));
  return returnVec;
}

void Experiments::Log(std::string message, bool debug) {
  /// @brief Log a message
  /// @param message std::string message to log
  /// @param debug bool whether to log or not
  if (debug) {
    std::cout << "[[LOG]] " << message << std::endl;
  }
}

int Experiments::mean(std::vector<int> vec) {
  /// @brief Calculate the average of a vector
  /// @param vec std::vector<int> vector to calculate the average of
  /// @return int average of the vector

  std::cout << "vec.size(): " << vec.size() << std::endl;

  int sum = 0;
  for (int i = 0; i < vec.size(); i++) {
    sum += vec[i];
  }
  std::cout << "sum: " << sum << std::endl;
  return sum / vec.size();
}

int Experiments::variance(std::vector<int> vec) {
  /// @brief Calculate the variance of a vector
  /// @param vec std::vector<int> vector to calculate the variance of
  /// @return int variance of the vector
  int avg = mean(vec);
  int sum = 0;
  for (int i = 0; i < vec.size(); i++) {
    sum += (vec[i] - avg) * (vec[i] - avg);
  }
  return sum / vec.size();
}
