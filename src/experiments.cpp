
#include "experiments.hpp"

#include <fstream>
#include <future>
#include <thread>
#include <vector>

#include "player.hpp"
#include "utilities.hpp"

Experiments::Experiments() {
  /// @brief Constructor for Experiments
  Utilities util;
  Utilities::Debug("Experiments constructor called");
}

Experiments::~Experiments() {
  /// @brief Destructor for Experiments
  Utilities::Debug("Experiments destructor called");
}

void Experiments::spotTest(std::unique_ptr<Player>& player1, std::unique_ptr<Player>& player2, int iterations, bool visual,
                           std::vector<int>& results, std::vector<int>& visualResults, std::mutex& mtx, std::mutex& visualMtx) {
  /// @brief see how many cards it takes for hand1 to beat hand2
  /// @param hand1 std::shared_ptr<Hand> hand1
  /// @param hand2 std::shared_ptr<Hand> hand2
  int ahead = TIE;

  // start timer
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  ahead = Utilities::compareHand(player1->hand->value, player2->hand->value);
  if (ahead == TIE) {
    Utilities::Debug("Tie");
    return;
  } else if (ahead == PLAYER2_AHEAD) {  // not sure if should do this
    return spotTest(player2, player1, iterations, visual, results, visualResults, mtx, visualMtx);
  }

  int beforeTricks = player2->tricks;
  std::vector<int> beforePlayOrder = player2->playOrder;

  for (int _ = 0; _ < iterations; _++) {
    int count = 0;
    ahead = Utilities::compareHand(player1->hand->value, player2->hand->value);

    while (ahead == PLAYER1_AHEAD) {
      player2->playCard();
      count += 1;
      player2->evaluateHand();
      ahead = Utilities::compareHand(player1->hand->value, player2->hand->value);
    }
    player2->printHand(player2->evaluateHand());
    mtx.lock();
    results.push_back(count);
    mtx.unlock();

    if (visual) {
      visualMtx.lock();
      visualResults[count] += 1;
      visualMtx.unlock();
    }

    Utilities::Debug("iteration: " + std::to_string(_));

    Utilities::Debug("--------------------------------");
    player2->setState(beforeTricks, beforePlayOrder);
    player2->evaluateHand();

    ahead = Utilities::compareHand(player1->hand->value, player2->hand->value);
  }

  // end timer
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  Utilities::Log("Time difference = " + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) +
                 "[ms]");
}

std::vector<double> Experiments::spotTestp(std::unique_ptr<Player>& player1, std::unique_ptr<Player>& player2, int iterations,
                                           bool visual) {
  /// @brief see how many cards it takes for hand1 to beat hand2
  /// @param hand1 std::shared_ptr<Hand> hand1
  /// @param hand2 std::shared_ptr<Hand> hand2
  int ahead = TIE;

  // start timer
  std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

  std::vector<int> results;
  std::mutex mtx;

  std::vector<int> visualResults = std::vector<int>(DECK_SIZE);
  std::mutex visualMtx;

  std::vector<std::unique_ptr<Player>> player1s;
  std::vector<std::unique_ptr<Player>> player2s;

  for (int i = 0; i < iterations; i++) {
    player1s.push_back(std::make_unique<Player>(player1));
    player2s.push_back(std::make_unique<Player>(player2));
  }

  unsigned int numThreads = std::thread::hardware_concurrency();
  Utilities::Debug("Number of threads: " + std::to_string(numThreads));
  std::vector<std::thread> threads;

  for (int i = 0; i < numThreads; i++) {
    threads.push_back(std::thread(&Experiments::spotTest, this, std::ref(player1s[i]), std::ref(player2s[i]), iterations / numThreads,
                                  visual, std::ref(results), std::ref(visualResults), std::ref(mtx), std::ref(visualMtx)));
  }

  for (int i = 0; i < numThreads; i++) {
    threads[i].join();
  }

  // write to file
  if (visual) {
    std::ofstream file;
    file.open("../data/results.txt");
    for (int i = 0; i < DECK_SIZE; i++) {
      file << visualResults[i] << std::endl;
    }
    file.close();
  }

  std::vector<double> returnVec;
  returnVec.push_back(mean(results));
  returnVec.push_back(variance(results));

  // end timer
  std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  Utilities::Log("Time difference = " + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()) +
                 "[ms]");

  return returnVec;
}

double Experiments::mean(std::vector<int> vec) {
  /// @brief Calculate the average of a vector
  /// @param vec std::vector<int> vector to calculate the average of
  /// @return int average of the vector

  double sum = 0;
  for (int i = 0; i < vec.size(); i++) {
    sum += static_cast<double>(vec[i]);
  }
  return sum / vec.size();
}

double Experiments::variance(std::vector<int> vec) {
  /// @brief Calculate the variance of a vector
  /// @param vec std::vector<int> vector to calculate the variance of
  /// @return int variance of the vector
  double avg = mean(vec);
  double sum = 0;
  for (int i = 0; i < vec.size(); i++) {
    sum += static_cast<double>(vec[i] - avg) * static_cast<double>(vec[i] - avg);
  }
  return sum / vec.size();
}
