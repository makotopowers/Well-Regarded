#include "player.hpp"

#include <queue>
#include <vector>

Player::Player() {
  std::cout << "Player constructor called" << std::endl;
  hand = new Hand();
  cardsLeft = std::set<int>();
  for (int i = 0; i < 54; i++) {
    cardsLeft.insert(i);
  }
}

Player::~Player() {
  std::cout << "Player destructor called" << std::endl;
  delete hand;
}

void Player::evaluateHand(Hand *hand) {
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

  std::queue<int> straight;
  std::vector<std::priority_queue<int, std::vector<int>, std::greater<int>>>
      flushes;
  std::priority_queue<int, std::vector<int>, std::greater<int>> flushC;
  std::priority_queue<int, std::vector<int>, std::greater<int>> flushD;
  std::priority_queue<int, std::vector<int>, std::greater<int>> flushH;
  std::priority_queue<int, std::vector<int>, std::greater<int>> flushS;
  flushes = {flushC, flushD, flushH, flushS};

  for (int i = 0; i < hand->cards[0].size(); i++) {
    int count = 0;
    for (int j = 0; j < hand->cards.size(); j++) {
      if (hand->cards[j][i]) {
        count++;
        flushes[j].push(i + 1);
        if (flushes[j].size() > 5) {
          flushes[j].pop();
        }
      }
    }
    switch (count) {
      case 0:
        continue;
      case 4:
        quads = i + 1;
        break;
      case 3:
        if (trips > 0 && trips > pair) {
          pair = trips;
          trips = i + 1;
          fullHouse = trips;
        } else if (pair > 0) {
          trips = i + 1;
          fullHouse = i + 1;
        } else {
          trips = i + 1;
        }
        break;
      case 2:
        if (pair > 0) {
          twoPair = i + 1;
        } else {
          pair = i + 1;
        }
        break;
      default:
        highCards.push(i + 1);
        break;
    }

    if (count > 0) {
      if (straight.empty()) {
        straight.push(i + 1);
      } else if (straight.back() == i) {
        straight.push(i + 1);
      } else {
        straight = std::queue<int>();
        straight.push(i + 1);
      }
      if (straight.size() > 5) straight.pop();
    }
    if (straight.size() > 4) straight_ = straight.back();

    for (int j = 0; j < hand->cards.size(); j++) {
      if (flushes[j].size() > 4) {
        flush = flushes[j].top() + 4;
        if (straight_ == flush) {
          straightFlush = flush;
        }
      }
    }
  }

  hand->flush = flush;
  hand->fullHouse = fullHouse;
  hand->quads = quads;
  hand->pair = pair;
  hand->straight_ = straight_;
  hand->straightFlush = straightFlush;
  hand->trips = trips;
  hand->twoPair = twoPair;
  hand->quint = quint;
}

void Player::addCard(int card) {
  int suit = card / 13;
  int value = card % 13;
  if (value == 0) {
    hand->cards[suit][0] = true;
    hand->cards[suit][13] = true;
  } else {
    hand->cards[suit][value] = true;
  }
}

int Player::turn(Hand *handV, int tricksV, int jokersV, int numCardsLeftV) {
  if (this->shouldPlay(handV, tricksV, jokersV, numCardsLeftV) == 1) {
    while (this->hand->value < handV->value) {
      this->playCard();
      this->evaluateHand(this->hand);
    }
  }
  return 0;
}

int Player::shouldPlay(Hand *handV, int tricksV, int jokersV,
                       int numCardsLeftV) {
  // HELLA CRAZY MCTS AND AI STUFF
  return 0;
}

void Player::playCard() {
  while (true) {
    int card = rand() % 54;
    if (cardsLeft.count(card) == 1) {
      this->addCard(card);
      cardsLeft.erase(card);
      break;
    }
  }
}