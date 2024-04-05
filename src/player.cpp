#include "player.hpp"

#include <queue>
#include <vector>

Player::Player() {
  std::cout << "Player constructor called" << std::endl;
  hand = new Hand();
  cardsLeft = std::set<int>();
  for (int i = 0; i < 52; i++) {
    cardsLeft.insert(i);
  }
}

Player::Player(std::string name) {
  this->name = name;
  std::cout << "Player constructor called" << std::endl;
  std::cout << "My name is " << this->name << std::endl;
  hand = new Hand();
  cardsLeft = std::set<int>();
  for (int i = 0; i < 52; i++) {
    cardsLeft.insert(i);
  }
}

Player::~Player() {
  std::cout << "Player destructor called" << std::endl;
  delete hand;
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
  int yield = 0;
  if (cardsLeft.size() < 1) {
    return 1;
  }
  if (this->shouldPlay(handV, tricksV, jokersV, numCardsLeftV) == 1) {
    while (this->hand->value < handV->value) {
      yield = this->playCard();
      if (yield == 1) {
        break;
      }
      this->Log("Card played");
      evaluateHand(this->hand);
      printHand(this->hand->value);
    }
  } else {
    yield = 1;
  }
  return yield;
}

int Player::shouldPlay(Hand *handV, int tricksV, int jokersV,
                       int numCardsLeftV) {
  // HELLA CRAZY MCTS AND AI STUFF
  return 1;
}

int Player::playCard() {
  if (cardsLeft.size() < 1) {
    return 1;
  }
  std::vector<int> cardsLeftVector =
      std::vector<int>(cardsLeft.begin(), cardsLeft.end());
  std::srand(static_cast<unsigned int>(std::time(nullptr) + rand()));
  int card = std::rand() % cardsLeft.size();
  std::cout << this->name << ": " << cardsLeft.size() << " Cards Left"
            << std::endl;
  this->addCard(cardsLeftVector[card]);
  this->printCard(cardsLeftVector[card]);
  cardsLeft.erase(cardsLeftVector[card]);

  return 0;
}

void Player::resetHand() {
  delete hand;
  this->hand = new Hand();
}

void Player::printCard(int card) {
  int suit = (card) / 13;
  int value = (card) % 13;
  std::string suits[4] = {"c", "d", "h", "s"};
  std::string values[13] = {"A", "2", "3",  "4", "5", "6", "7",
                            "8", "9", "10", "J", "Q", "K"};
  std::cout << values[value] << suits[suit] << std::endl;
}

void Player::printHand(std::vector<int> hand) {
  int h = hand[0];
  std::string values[10] = {
      "High Card",      "Pair",  "Two Pair",   "Three of a Kind",
      "Straight",       "Flush", "Full House", "Four of a Kind",
      "Straight Flush", "Quint"};
  std::cout << values[h] << " ";
  for (int i = 1; i < hand.size(); i++) {
    std::cout << hand[i] << " ";
  }

  std::cout << std::endl;
}

void Player::Log(std::string message, bool debug) {
  if (debug) {
    std::cout << message << std::endl;
  }
}

std::vector<int> Player::evaluateHand(Hand *hand) {
  bool debug = false;

  std::priority_queue<int, std::vector<int>> highCards;
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

  int flushIndex = -1;
  std::vector<int> flushC = std::vector<int>();
  std::vector<int> flushD = std::vector<int>();
  std::vector<int> flushH = std::vector<int>();
  std::vector<int> flushS = std::vector<int>();
  std::vector<std::vector<int>> flushes = {flushC, flushD, flushH, flushS};

  std::vector<int> handValue;

  for (int i = 0; i < 14; i++) {
    int count = 0;
    for (int j = 0; j < 4; j++) {
      if (hand->cards[j][i] == 1) {
        count++;
        flushes[j].push_back(i + 1);
      }
    }
    if (i != 0) {
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

    for (int j = 0; j < 4; j++) {
      if (flushes[j].size() < 5) {
        continue;
      }
      if (flushes[j].size() > 4) {
        if (flushIndex == -1) {
          flushIndex = j;
        } else {
          for (int k = 0; k < 5; k++) {
            if (flushes[j][flushes[j].size() - 1 - k] >
                flushes[flushIndex][flushes[flushIndex].size() - 1 - k]) {
              flushIndex = j;
            } else if (flushes[j][flushes[j].size() - 1 - k] <
                       flushes[flushIndex]
                              [flushes[flushIndex].size() - 1 - k]) {
              break;
            }
          }
        }
        flush = flushes[flushIndex][flushes[flushIndex].size() - 1];

        if (straight_ == flush) {
          straightFlush = flush;
        }
      }
    }
  }
  if (quint > 0) {
    handValue.push_back(9);
    handValue.push_back(quint);
  } else if (straightFlush > 0) {
    handValue.push_back(8);
    handValue.push_back(straightFlush);
  } else if (quads > 0) {
    handValue.push_back(7);
    handValue.push_back(quads);
    if (highCards.size() > 0) {
      handValue.push_back(highCards.top());
    }
  } else if (fullHouse > 0) {
    handValue.push_back(6);
    handValue.push_back(trips);
    handValue.push_back(pair);
  } else if (flush > 0) {
    handValue.push_back(5);
    for (int i = 0; i < 5; i++) {
      handValue.push_back(
          flushes[flushIndex][flushes[flushIndex].size() - 1 - i]);
    }
  } else if (straight_ > 0) {
    handValue.push_back(4);
    handValue.push_back(straight_);
  } else if (trips > 0) {
    handValue.push_back(3);
    handValue.push_back(trips);
    while (highCards.size() > 2) highCards.pop();
    while (!highCards.empty()) {
      handValue.push_back(highCards.top());
      highCards.pop();
    }
    std::reverse(handValue.end() - 2, handValue.end());
  } else if (twoPair > 0) {
    handValue.push_back(2);
    handValue.push_back(twoPair);
    handValue.push_back(pair);
    handValue.push_back(highCards.top());
  } else if (pair > 0) {
    handValue.push_back(1);
    handValue.push_back(pair);
    for (int i = 0; i < 3; i++) {
      if (highCards.size() > 0) {
        handValue.push_back(highCards.top());
        highCards.pop();
      }
    }
  } else {
    handValue.push_back(0);
    for (int i = 0; i < 5; i++) {
      if (highCards.size() > 0) {
        handValue.push_back(highCards.top());
        highCards.pop();
      }
    }
  }
  while (handValue.size() < 6) {
    handValue.push_back(0);
  }

  hand->value = handValue;
  return handValue;
}
