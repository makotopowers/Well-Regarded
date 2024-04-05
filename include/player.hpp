#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <vector>

enum class Suit { C, D, H, S };

struct Hand {
  std::vector<std::vector<bool>> cards;
  std::vector<bool> cardsC;
  std::vector<bool> cardsD;
  std::vector<bool> cardsH;
  std::vector<bool> cardsS;
  std::vector<int> value;

  Hand() {
    std::cout << "Hand constructor called" << std::endl;
    cardsC = std::vector<bool>(14, false);
    cardsD = std::vector<bool>(14, false);
    cardsH = std::vector<bool>(14, false);
    cardsS = std::vector<bool>(14, false);
    cards = {cardsC, cardsD, cardsH, cardsS};
  }
};

class Player {
 public:
  Player();
  Player(std::string name);
  ~Player();
  std::string name;

  Hand *hand;
  int jokers;
  int tricks;
  std::set<int> cardsLeft;
  int numCardsLeft;

  void addCard(int card);
  void addCard(std::string card);
  void Log(std::string message, bool debug = true);

  void resetHand();
  void printCard(int card);
  void printHand(std::vector<int> hand);

  std::vector<int> evaluateHand(Hand *hand);
  std::vector<int> test_evaluateHand(Hand *hand);
  int turn(Hand *handV, int tricksV, int jokersV, int numCardsLeftV);
  int shouldPlay(Hand *handV, int tricksV, int jokersV, int numCardsLeftV);
  int playCard();
};

#endif  // PLAYER_HPP
