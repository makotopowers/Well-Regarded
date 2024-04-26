#include "player.hpp"

#include <algorithm>
#include <numeric>
#include <queue>
#include <vector>

#define debugPrint(x) std::cout << "[[DEBUG]] " << x << std::endl;
// #define debugPrint(x)

Player::Player() {
  /// @brief Default constructor for Player

  std::cout << "Player constructor called" << std::endl;
  hand = std::make_shared<Hand>();
  cardsLeft = std::set<int>();
  for (int i = 0; i < 54; i++) {
    cardsLeft.insert(i);
  }
}

Player::Player(std::string name) {
  /// @brief Constructor for Player with name
  /// @param name Name of the player

  this->name = name;
  std::cout << "Player constructor called" << std::endl;
  std::cout << "My name is " << this->name << std::endl;
  hand = std::make_shared<Hand>();
  cardsLeft = std::set<int>();
  for (int i = 0; i < 54; i++) {
    cardsLeft.insert(i);
  }
}

Player::~Player() {
  /// @brief Destructor for Player
  std::cout << "Player destructor called" << std::endl;
}

void Player::addCard(int card, bool joker) {
  /// @brief Add card to player's hand
  /// @param card Card to add
  /// @param joker Whether the card is a joker

  debugPrint("Adding card to hand " + std::to_string(card));
  if (card == 52 || card == 53) {
    hand->jokerAvailable = true;
    hand->jokers++;
  } else {
    int suit = card / 13;
    int value = card % 13;
    if (value == 0) {
      hand->cards[suit][0] += 1;
      hand->cards[suit][13] += 1;
    } else {
      hand->cards[suit][value] += 1;
    }
  }
}

int Player::turn(std::shared_ptr<Hand> handV, int tricksV, int jokersV,
                 int numCardsLeftV) {
  /// @brief Player's turn
  /// @param handV Opponent's hand
  /// @param tricksV Opponent's tricks
  /// @param jokersV Opponent's jokers
  /// @param numCardsLeftV Opponent's cards left
  /// @return 1 if player should yield, 0 if player should play

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

int Player::shouldPlay(std::shared_ptr<Hand> handV, int tricksV, int jokersV,
                       int numCardsLeftV) {
  /// @brief Determine if player should play
  /// @param handV Opponent's hand
  /// @param tricksV Opponent's tricks
  /// @param jokersV Opponent's jokers
  /// @param numCardsLeftV Opponent's cards left
  /// @return 1 if player should yield, 0 if player should play

  // HELLA CRAZY MCTS AND AI STUFF
  return 1;
}

int Player::playCard() {
  /// @brief Play a card
  /// @return 1 if player should yield, 0 if player should play

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
  /// @brief Reset player's hand
  debugPrint(this->hand.use_count());  // NOTE: this should be 1
  this->hand.reset();
  this->hand = std::make_shared<Hand>();
}

void Player::printCard(int card) {
  /// @brief Print card
  /// @param card Card to print

  if (card == 52) {
    std::cout << "JokerA" << std::endl;
    return;
  } else if (card == 53) {
    std::cout << "JokerB" << std::endl;
    return;
  }

  int suit = (card) / 13;
  int value = (card) % 13;
  std::string suits[4] = {"c", "d", "h", "s"};
  std::string values[13] = {"A", "2", "3",  "4", "5", "6", "7",
                            "8", "9", "10", "J", "Q", "K"};
  std::cout << values[value] << suits[suit] << std::endl;
}

void Player::printHand(std::vector<int> hand) {
  /// @brief Print hand
  /// @param hand Hand to print

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
  /// @brief Log message
  /// @param message Message to log
  /// @param debug Whether to log in debug mode

  if (debug) {
    std::cout << message << std::endl;
  }
}

std::vector<int> Player::evaluateHand(std::shared_ptr<Hand> hand) {
  /// @brief Evaluate player's hand
  /// @param hand Player's hand
  /// @return Hand value

  std::cout << "Starting Hand Evaluation" << std::endl;
  bool debug = false;

  debugPrint("Evaluating hand");

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

  bool jokerAvailable = hand->jokerAvailable;
  std::cout << "Joker Available: " << jokerAvailable << std::endl;
  bool jokerEval = jokerAvailable;

  int currJokerValue = 0;
  int jokerSuit = 0;
  int jokerHandStrength = -1;
  int maxFlushSizeIndex = 0;

  std::vector<int> straight;

  int flushIndex = -1;
  std::vector<int> flushC = std::vector<int>();
  std::vector<int> flushD = std::vector<int>();
  std::vector<int> flushH = std::vector<int>();
  std::vector<int> flushS = std::vector<int>();
  std::vector<std::vector<int>> flushes = {flushC, flushD, flushH, flushS};

  std::vector<int> handValue;

  int countMax = 0;
  int countIndex = 0;

  debugPrint("Starting for loop");
  for (int i = 0; i < 14; i++) {
    int count = 0;
    for (int j = 0; j < 4; j++) {
      if (hand->cards[j][i] > 0) {
        count += hand->cards[j][i];
        flushes[j].push_back(i + 1);
      }
    }

    if (count > countMax) {  // for checking where the joker should go later on
      countMax = count;
      countIndex = i;
    }

    debugPrint("Checkpoint A");
    debugPrint("Count: " + std::to_string(count) + " for " + std::to_string(i));
    if (i != 0) {  // initial switch to see what hands we have made
      switch (count) {
        case 0:
          continue;
        case 4:
          quads = i + 1;
          break;
        case 3:
          std::cout << "Trips: " << trips << std::endl;
          std::cout << "Pair: " << pair << std::endl;

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
          if (trips > 0) {
            fullHouse = trips;
            pair = i + 1;
          } else if (twoPair > 0) {
            pair = twoPair;
            twoPair = i + 1;
          } else if (pair > 0) {
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

    debugPrint("Checkpoint B");
    if (count > 0) {
      straight.push_back(i + 1);
      if (straight.size() > 4) {
        if (straight[straight.size() - 1] - straight[straight.size() - 5] ==
            4) {
          straight_ = straight.back();
        }
      }
      if (straight.size() > 3 && jokerEval) {
        if (straight[straight.size() - 1] - straight[straight.size() - 4] ==
            4) {
          currJokerValue =
              (straight[straight.size() - 1] * straight[straight.size() - 1] -
               1) /
                  2 -
              (straight[straight.size() - 4] * straight[straight.size() - 4] -
               1) /
                  2 -
              std::accumulate(straight.end() - 4, straight.end(), 0);
          straight_ = straight.back();
          jokerHandStrength = 4;
        } else if (straight[straight.size() - 1] -
                       straight[straight.size() - 4] ==
                   3) {
          currJokerValue = straight[straight.size() - 1] == 14
                               ? 10
                               : straight[straight.size() - 1] + 1;
          straight_ = std::max(straight.back(), currJokerValue);
          jokerHandStrength = 4;
        }
      }

      jokerAvailable = jokerEval;

      debugPrint("Checkpoint B");

      for (int j = 0; j < 4; j++) {
        maxFlushSizeIndex =
            flushes[j].size() > flushes[maxFlushSizeIndex].size()
                ? j
                : maxFlushSizeIndex;
        if (flushes[j].size() > 3 && jokerEval) {
          if (flushes[j][flushes[j].size() - 1] -
                  flushes[j][flushes[j].size() - 4] ==
              4) {
            currJokerValue =
                (flushes[j][flushes[j].size() - 1] *
                     flushes[j][flushes[j].size() - 1] -
                 1) /
                    2 -
                (flushes[j][flushes[j].size() - 5] *
                     flushes[j][flushes[j].size() - 5] -
                 1) /
                    2 -
                std::accumulate(flushes[j].end() - 5, flushes[j].end(), 0);
            straightFlush = flushes[j][flushes[j].size() - 1];
            jokerSuit = j;
            jokerHandStrength = 8;
          } else if (flushes[j][flushes[j].size() - 1] -
                         flushes[j][flushes[j].size() - 4] ==
                     3) {
            currJokerValue = flushes[j][flushes[j].size() - 1] == 14
                                 ? 10
                                 : flushes[j][flushes[j].size() - 1] + 1;
            straightFlush = std::max(currJokerValue, flushes[j].back());
            jokerSuit = j;
            jokerHandStrength = 8;
          }
        }
        if (flushes[j].size() > 4) {
          if (flushes[j][flushes[j].size() - 1] -
                  flushes[j][flushes[j].size() - 5] ==
              4) {
            straightFlush = flushes[j][flushes[j].size() - 1];
          }

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
        }
      }
    }
  }

  debugPrint("finished for loop");

  if (jokerEval && jokerHandStrength < 5 &&
      flushes[maxFlushSizeIndex].size() > 3) {
    if (flush != 0) {
      flushes[flushIndex].push_back(14);
      jokerSuit = flushIndex;
      flush = 14;
    } else if (flushes[maxFlushSizeIndex].size() == 4) {
      flushes[maxFlushSizeIndex].push_back(14);
      jokerSuit = maxFlushSizeIndex;
      flush = 14;
      flushIndex = maxFlushSizeIndex;
    }
    currJokerValue = 14;
    jokerHandStrength = 5;
  }
  if (jokerEval && jokerHandStrength < 5) {
    jokerSuit = maxFlushSizeIndex;
  }

  debugPrint("Checking Joker Hands");
  if (jokerAvailable) {
    debugPrint("CountMax" + std::to_string(countMax) + " CountIndex " +
               std::to_string(countIndex));
    countIndex = countMax == 0 ? 13 : countIndex;
    switch (countMax + 1) {
      case 5:
        quint = countIndex + 1;
        break;
      case 4:
        if (jokerHandStrength < 8) {
          quads = countIndex + 1;
          currJokerValue = quads;
          jokerHandStrength = 7;
        }
        break;
      case 3:
        if (jokerHandStrength < 4) {
          if (trips > 0 && trips > pair) {
            pair = trips;
            trips = countIndex + 1;
            fullHouse = trips;
            jokerHandStrength = 6;
          } else if (pair > 0) {
            trips = countIndex + 1;
            fullHouse = countIndex + 1;
            jokerHandStrength = 6;
          } else {
            trips = countIndex + 1;
            jokerHandStrength = 3;
          }
          currJokerValue = trips;
        }
        break;
      case 2:
        debugPrint("Case 2, jokerHandStrength: " +
                   std::to_string(jokerHandStrength));
        if (jokerHandStrength < 7 && trips > 0) {
          fullHouse = trips;
          pair = countIndex + 1;
        } else if (jokerHandStrength < 3) {
          if (twoPair > 0) {
            pair = twoPair;
            twoPair = countIndex + 1;
            jokerHandStrength = 2;
            currJokerValue = twoPair;
          } else if (pair > 0) {
            twoPair = countIndex + 1;
            jokerHandStrength = 2;
            currJokerValue = twoPair;
          } else {
            pair = countIndex + 1;
            jokerHandStrength = 1;
            currJokerValue = pair;
          }
        }
        break;
      case 1:
        if (jokerHandStrength < 1) highCards.push(14);
        currJokerValue = 14;
        jokerHandStrength = 0;
        break;

      default:
        break;
    }
  }

  debugPrint("Checking Normal Hands");

  if (straightFlush > 0) {
    handValue.push_back(8);
    handValue.push_back(straightFlush);
  } else if (quads > 0) {
    handValue.push_back(7);
    handValue.push_back(quads);
    if (highCards.size() > 0) {
      handValue.push_back(std::max(highCards.top(), std::max(pair, trips)));
    } else {
      handValue.push_back(std::max(pair, trips));
    }

  } else if (fullHouse > 0) {
    handValue.push_back(6);
    handValue.push_back(trips);
    handValue.push_back(std::max(pair, twoPair));
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
    for (int i = 0; i < 2; i++) {
      if (highCards.size() > 0) {
        handValue.push_back(highCards.top());
        highCards.pop();
      }
    }
  } else if (twoPair > 0) {
    debugPrint("Two Pair: " + std::to_string(twoPair));
    handValue.push_back(2);
    handValue.push_back(twoPair);
    handValue.push_back(pair);
    bool add = true;
    while (add) {
      if (highCards.size() == 0) {
        break;
      }
      if (highCards.size() > 0) {
        if (pair == highCards.top() || twoPair == highCards.top()) {
          highCards.pop();
          continue;
        } else {
          handValue.push_back(highCards.top());
          add = false;
        }
      }
    }

  } else if (pair > 0) {
    handValue.push_back(1);
    handValue.push_back(pair);
    for (int i = 0; i < 3;) {
      if (highCards.size() == 0) {
        break;
      }
      if (highCards.size() > 0) {
        if (pair == highCards.top()) {
          highCards.pop();
          continue;
        } else {
          handValue.push_back(highCards.top());
          highCards.pop();
          i++;
        }
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
  debugPrint("Finished Normal Hands");
  while (handValue.size() < 6) {
    handValue.push_back(0);
  }

  if (jokerHandStrength != 5 && jokerHandStrength != 8) {
    jokerSuit = maxFlushSizeIndex;
  }
  if (jokerEval) {
    if (currJokerValue == 14) {
      currJokerValue = 1;
    }
    std::cout << "Joker Evaluated" << std::endl;
    std::cout << "Joker Suit: " << jokerSuit << std::endl;
    std::cout << "Joker Value: " << currJokerValue << std::endl;
    printCard(jokerSuit * 13 + currJokerValue - 1);
    this->addCard(jokerSuit * 13 + currJokerValue - 1, true);
  }

  debugPrint("Hand Evaluation Finished");
  hand->jokerAvailable = false;

  std::cout << "handValue: " << std::endl;
  for (auto _ : handValue) {
    std::cout << _ << " ";
  }
  std::cout << std::endl;

  hand->value = handValue;
  return handValue;
}
