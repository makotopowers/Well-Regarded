#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>
#include <set>
#include <string>
#include <vector>

enum class Suit { C, D, H, S };

struct Hand {
  std::vector<std::vector<int>> cards;
  std::vector<int> cardsC;
  std::vector<int> cardsD;
  std::vector<int> cardsH;
  std::vector<int> cardsS;
  std::vector<int> value;
  int jokers;
  bool jokerAvailable;

  Hand() {
    std::cout << "Hand constructor called" << std::endl;
    cardsC = std::vector<int>(14, 0);
    cardsD = std::vector<int>(14, 0);
    cardsH = std::vector<int>(14, 0);
    cardsS = std::vector<int>(14, 0);
    cards = {cardsC, cardsD, cardsH, cardsS};
  }
};

class Player {
 public:
  Player();
  Player(std::string name);
  ~Player();
  std::string name;

  std::shared_ptr<Hand> hand;

  int tricks;
  int jokers;
  std::set<int> cardsLeft;
  int numCardsLeft;
  std::vector<int> playOrder;

  void addCard(int card);
  void addCard(std::string card);
  void Log(std::string message, bool debug = true);
  void setState(int tricks_, std::vector<int> playOrder_);

  void resetHand();
  void resetPlayer();
  void printCard(int card);
  void printHand(std::vector<int> hand);

  std::vector<int> evaluateHand(std::shared_ptr<Hand> hand = nullptr);
  // std::vector<int> evaluateHand();
  int turn(std::shared_ptr<Hand> handV, int tricksV, int jokersV, int numCardsLeftV);
  int shouldPlay(std::shared_ptr<Hand> handV, int tricksV, int jokersV, int numCardsLeftV);
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
                       int currJokerValue, int jokerHandStrength, int JokerEval);
};

#endif  // PLAYER_HPP
