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

  std::priority_queue<int, std::vector<int>, std::greater<int>> highCards;
  int pair = 0;
  int twoPair = 0;
  int trips = 0;
  int straight_ = 0;
  int flush = 0;
  int fullHouse = 0;
  int quads = 0;
  int straightFlush = 0;
  int quint = 0;

  int value;

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
  ~Player();

  Hand *hand;
  int jokers;
  int tricks;
  std::set<int> cardsLeft;
  int numCardsLeft;

  void addCard(int card);
  void addCard(std::string card);
  void printHand();

  void evaluateHand(Hand *hand);

  int turn(Hand *handV, int tricksV, int jokersV, int numCardsLeftV);
  int shouldPlay(Hand *handV, int tricksV, int jokersV, int numCardsLeftV);
  void playCard();
};

#endif  // PLAYER_HPP
