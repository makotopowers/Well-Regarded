#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>
#include <set>
#include <string>
#include <vector>
#include "utilities.hpp"

enum class Suit { C, D, H, S };

struct Hand {
  std::vector<std::vector<int>> cards;
  std::vector<int> value;
  int jokers;
  bool jokerAvailable;

  Hand() {
    Utilities::Debug("Hand constructor called");
    cards = {std::vector<int>(14, 0), std::vector<int>(14, 0), std::vector<int>(14, 0), std::vector<int>(14, 0)};
  }

  Hand(const std::unique_ptr<Hand>& hand_) {
    Utilities::Debug("Hand copy constructor called");
    for (auto i : hand_->cards[0]) {
      Utilities::Debug("i: " + std::to_string(i));
    }
    cards = hand_->cards;
    value = hand_->value;
    jokers = hand_->jokers;
    jokerAvailable = hand_->jokerAvailable;
  }

  Hand operator=(const Hand& hand_) = delete;
};

class Player {
 public:
  Player();
  Player(std::string name);
  Player(std::unique_ptr<Player>& player_);  // copy constructor
  Player& operator=(const Player& player_) = delete;
  ~Player();
  std::string name;

  std::unique_ptr<Hand> hand;

  int tricks;
  int jokers;
  std::set<int> cardsLeft;
  int numCardsLeft;
  std::vector<int> playOrder;

  void addCard(int card);

  void setState(int tricks_, std::vector<int> playOrder_, int jokersPlayed_, int numCardsLeft_);

  void resetHand();
  void resetPlayer();
  void printCard(int card);
  std::string printHand(std::vector<int> hand);

  std::vector<int> evaluateHand();
  std::vector<int> evaluateHand(std::unique_ptr<Hand>& hand);
  int turn(std::unique_ptr<Hand>& handV, int tricksV, int jokersV, int numCardsLeftV);
  int shouldPlay(std::unique_ptr<Hand>& handV, int tricksV, int jokersV, int numCardsLeftV);
  int playCard();

 private:
  inline int findMissing(std::vector<int>& vec) {

    return ((vec[vec.size() - 1]) * (vec[vec.size() - 1] + 1)) / 2 - (((vec[vec.size() - 4] - 1)) * ((vec[vec.size() - 4] - 1) + 1)) / 2 -
           std::accumulate(vec.end() - 4, vec.end(), 0);
  }
  void addToHand(int pair, int twoPair, int trips, int straight_, int flush, int fullHouse, int quads, int straightFlush, int quint,
                 std::priority_queue<int, std::vector<int>>& highCards, std::vector<int>& handValue, std::vector<std::vector<int>>& flushes,
                 int flushIndex);

  void straightEval(std::vector<int>& straight, int& straight_, bool jokerEval, int& currJokerValue, int& jokerHandStrength);
  void flushEval(int& maxFlushSizeIndex, bool jokerEval, std::vector<std::vector<int>>& flushes, int& currJokerValue, int& jokerSuit,
                 int& jokerHandStrength, int& straightFlush, int& flush, int& flushIndex);
  void NormieFlush(int jokerEval, int& jokerHandStrength, int& jokerSuit, int& flush, int& flushIndex, int& currJokerValue,
                   int& maxFlushSizeIndex, std::vector<std::vector<int>>& flushes);

  void checkCountHands(int count, int countIndex, int& pair, int& twoPair, int& trips, int& straight_, int& flush, int& fullHouse,
                       int& quads, int& straightFlush, int& quint, std::priority_queue<int, std::vector<int>>& highCards,
                       int& currJokerValue, int& jokerHandStrength, int JokerEval);
};

#endif  // PLAYER_HPP
