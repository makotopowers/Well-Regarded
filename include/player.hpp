#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
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
  int jokerValue;
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
  int jokers;
  int tricks;
  std::set<int> cardsLeft;
  int numCardsLeft;

  void addCard(int card, bool joker = false);
  void addCard(std::string card);
  void Log(std::string message, bool debug = true);

  void resetHand();
  void printCard(int card);
  void printHand(std::vector<int> hand);

  std::vector<int> evaluateHand(std::shared_ptr<Hand> hand);
  std::vector<int> test_evaluateHand(Hand *hand);
  int turn(std::shared_ptr<Hand> handV, int tricksV, int jokersV,
           int numCardsLeftV);
  int shouldPlay(std::shared_ptr<Hand> handV, int tricksV, int jokersV,
                 int numCardsLeftV);
  int playCard();
};

#endif  // PLAYER_HPP
