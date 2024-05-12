#include "player.hpp"
#include "utilities.hpp"

#include <queue>
#include <vector>

Player::Player() {
  /// @brief Default constructor for Player

  Utilities::Debug("Player constructor called");
  hand = std::make_unique<Hand>();
  cardsLeft = std::set<int>();
  for (int i = 0; i < 54; i++) {
    cardsLeft.insert(i);
  }
}

Player::Player(std::string name) {
  /// @brief Constructor for Player with name
  /// @param name Name of the player

  this->name = name;
  Utilities::Debug("Player constructor called");
  Utilities::Debug("My name is " + this->name);

  hand = std::make_unique<Hand>();
  cardsLeft = std::set<int>();
  for (int i = 0; i < 54; i++) {
    cardsLeft.insert(i);
  }
}

Player::Player(std::unique_ptr<Player>& player_) {
  /// @brief Copy constructor for Player
  /// @param player_ Player to copy

  Utilities::Debug("Player copy constructor called");
  this->name = player_->name;
  std::unique_ptr<Hand> hand_(new Hand(player_->hand));
  this->hand = std::move(hand_);
  this->tricks = player_->tricks;
  this->jokers = player_->jokers;
  this->cardsLeft = player_->cardsLeft;
  this->numCardsLeft = player_->numCardsLeft;
  this->playOrder = player_->playOrder;
}

Player::~Player() {
  /// @brief Destructor for Player
  Utilities::Debug("Player destructor called");
}

void Player::addCard(int card) {
  /// @brief Add card to player's hand
  /// @param card Card to add

  if (card > 53 || card < 0) {
    throw std::invalid_argument("Invalid card");
  }
  Utilities::Debug("Adding card to hand " + std::to_string(card));
  if (card == 52 || card == 53) {
    hand->jokerAvailable = true;
    this->jokers++;
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
  playOrder.push_back(card);
  cardsLeft.erase(card);
}

int Player::turn(std::unique_ptr<Hand>& handV, int tricksV, int jokersV, int numCardsLeftV) {
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
      Utilities::Debug("Card played");
      evaluateHand(this->hand);
      printHand(this->hand->value);
    }
  } else {
    yield = 1;
  }
  return yield;
}

int Player::shouldPlay(std::unique_ptr<Hand>& handV, int tricksV, int jokersV, int numCardsLeftV) {
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
  std::vector<int> cardsLeftVector = std::vector<int>(cardsLeft.begin(), cardsLeft.end());
  std::srand(static_cast<unsigned int>(std::time(nullptr) + rand()));
  int card = std::rand() % cardsLeft.size();
  Utilities::Debug(this->name + ": " + std::to_string(cardsLeft.size()) + " Cards Left");
  this->addCard(cardsLeftVector[card]);
  this->printCard(cardsLeftVector[card]);

  return 0;
}

void Player::resetHand() {
  /// @brief Reset player's hand

  this->hand.reset();
  this->hand = std::make_unique<Hand>();
  playOrder = std::vector<int>();
}

void Player::printCard(int card) {
  /// @brief Print card
  /// @param card Card to print

  if (card == 52) {

    Utilities::Debug("JokerA 52");
    return;
  } else if (card == 53) {
    Utilities::Debug("JokerB 53");
    return;
  }

  int suit = (card) / 13;
  int value = (card) % 13;
  std::string suits[4] = {"c", "d", "h", "s"};
  std::string values[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

  Utilities::Debug(values[value] + suits[suit] + " " + std::to_string(card));
}

std::string Player::printHand(std::vector<int> hand) {
  /// @brief Print hand
  /// @param hand Hand to print

  std::string ret;

  int h = hand[0];
  std::string values[10] = {"High Card", "Pair",       "Two Pair",       "Three of a Kind", "Straight",
                            "Flush",     "Full House", "Four of a Kind", "Straight Flush",  "Quint"};

  Utilities::Debug(values[h] + " ");
  ret += values[h] + " ";
  for (int i = 1; i < hand.size(); i++) {
    ret += std::to_string(hand[i]) + " ";

    Utilities::Debug(std::to_string(hand[i]));
  }
  return ret;
}

std::vector<int> Player::evaluateHand() {
  return evaluateHand(this->hand);
}
std::vector<int> Player::evaluateHand(std::unique_ptr<Hand>& hand) {
  /// @brief Evaluate player's hand
  /// @param hand Player's hand
  /// @return Hand value

  Utilities::Debug("Evaluating hand");

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

  bool jokerEval = hand->jokerAvailable;
  int currJokerValue = 0;
  int jokerSuit = 0;
  int jokerHandStrength = -1;
  int maxFlushSizeIndex = 0;
  int countMax = 0;
  int countIndex = 0;

  std::vector<int> straight;

  Utilities::Debug("jokerEval: " + std::to_string(jokerEval));

  int flushIndex = -1;
  std::vector<int> flushC = std::vector<int>();
  std::vector<int> flushD = std::vector<int>();
  std::vector<int> flushH = std::vector<int>();
  std::vector<int> flushS = std::vector<int>();
  std::vector<std::vector<int>> flushes = {flushC, flushD, flushH, flushS};

  std::vector<int> handValue;

  Utilities::Debug("Starting for loop");
  for (int i = 0; i < 14; i++) {
    int count = 0;
    for (int j = 0; j < 4; j++) {
      if (hand->cards[j][i] > 0) {
        count += hand->cards[j][i];
        if (i != 0) {
          flushes[j].push_back(i + 1);
        }
      }
    }
    if (count >= countMax) {  // for checking where the joker should go later on
      countMax = count;
      countIndex = i;
    }

    Utilities::Debug("Checkpoint A");
    if (i != 0) {  // initial switch to see what hands we have made
      checkCountHands(count, i, pair, twoPair, trips, straight_, flush, fullHouse, quads, straightFlush, quint, highCards, currJokerValue,
                      jokerHandStrength,
                      false);  // opportunity for optimization here, do not have to check every hand with every iteration of the loop
    }

    Utilities::Debug("Checkpoint B");

    if (count > 0) {
      straight.push_back(i + 1);
      straightEval(straight, straight_, jokerEval, currJokerValue, jokerHandStrength);
      flushEval(maxFlushSizeIndex, jokerEval, flushes, currJokerValue, jokerSuit, jokerHandStrength, straightFlush, flush, flushIndex);
      //technically could also optimize these, but gain is probably minimal and not worth the effort
    }
  }

  Utilities::Debug("finished for loop");

  NormieFlush(jokerEval, jokerHandStrength, jokerSuit, flush, flushIndex, currJokerValue, maxFlushSizeIndex, flushes);

  if (jokerEval) {
    countIndex = countMax == 0 ? 13 : countIndex;
    checkCountHands(countMax + 1, countIndex, pair, twoPair, trips, straight_, flush, fullHouse, quads, straightFlush, quint, highCards,
                    currJokerValue, jokerHandStrength, jokerEval);
  }

  addToHand(pair, twoPair, trips, straight_, flush, fullHouse, quads, straightFlush, quint, highCards, handValue, flushes, flushIndex);

  if (jokerHandStrength != 5 && jokerHandStrength != 8) {
    jokerSuit = maxFlushSizeIndex;
  }
  if (jokerEval) {
    currJokerValue = currJokerValue == 14 ? 1 : currJokerValue;
    Utilities::Debug("Joker Suit: " + std::to_string(jokerSuit));
    Utilities::Debug("Joker Value: " + std::to_string(currJokerValue));
    this->addCard(jokerSuit * 13 + currJokerValue - 1);
  }

  Utilities::Debug("Hand Evaluation Finished");
  hand->jokerAvailable = false;
  hand->value = handValue;
  return handValue;
}

void Player::setState(int tricks_, std::vector<int> playOrder_, int jokersPlayed_, int numCardsLeft_) {
  /// @brief Set player's hand
  /// @param playOrder_ Play order up to some point in the game

  this->resetPlayer();

  this->tricks = tricks_;
  std::unique_ptr<Hand> hand_ = std::make_unique<Hand>();
  this->hand.swap(hand_);
  this->jokers = jokersPlayed_;
  this->hand->jokers = jokersPlayed_;

  for (int i = 0; i < jokersPlayed_; i++) {
    this->cardsLeft.erase(52 + i);
    this->numCardsLeft--;
  }

  for (int i = 0; i < 53 - (numCardsLeft_ - jokersPlayed_); i++) {
    std::srand(static_cast<unsigned int>(std::time(nullptr) + rand()));
    int card = std::rand() % cardsLeft.size();
    if (std::find(playOrder_.begin(), playOrder_.end(), card) != playOrder_.end()) {
      this->cardsLeft.erase(card);
      this->numCardsLeft--;
    }
  }

  for (int i = 0; i < playOrder_.size(); i++) {
    this->addCard(playOrder_[i]);
    if (playOrder_[i] == 52 || playOrder_[i] == 53) {
      this->evaluateHand();
    }
  }
  this->playOrder = playOrder_;
}

void Player::resetPlayer() {
  /// @brief Reset player

  this->tricks = 0;
  this->playOrder.clear();
  this->hand.reset();
  std::unique_ptr<Hand> hand_ = std::make_unique<Hand>();
  this->hand.swap(hand_);
  this->cardsLeft.clear();
  for (int i = 0; i < 54; i++) {
    cardsLeft.insert(i);
  }
  this->numCardsLeft = 54;
  this->jokers = 0;
}

void Player::addToHand(int pair, int twoPair, int trips, int straight_, int flush, int fullHouse, int quads, int straightFlush, int quint,
                       std::priority_queue<int, std::vector<int>>& highCards, std::vector<int>& handValue,
                       std::vector<std::vector<int>>& flushes, int flushIndex) {
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
      handValue.push_back(flushes[flushIndex][flushes[flushIndex].size() - 1 - i]);
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
  Utilities::Debug("Finished Normal Hands");
  while (handValue.size() < 6) {
    handValue.push_back(0);
  }
}

void Player::straightEval(std::vector<int>& straight, int& straight_, bool jokerEval, int& currJokerValue, int& jokerHandStrength) {

  if (straight.size() > 4) {
    if (straight[straight.size() - 1] - straight[straight.size() - 5] == 4) {
      straight_ = straight.back();
    }
  }
  if (straight.size() > 3 && jokerEval) {
    if (straight[straight.size() - 1] - straight[straight.size() - 4] == 4) {
      currJokerValue = findMissing(straight);
      straight_ = straight.back();
      jokerHandStrength = 4;
    } else if (straight[straight.size() - 1] - straight[straight.size() - 4] == 3) {
      currJokerValue = straight[straight.size() - 1] == 14 ? 10 : straight[straight.size() - 1] + 1;
      straight_ = std::max(straight.back(), currJokerValue);
      jokerHandStrength = 4;
    }
  }
}

void Player::flushEval(int& maxFlushSizeIndex, bool jokerEval, std::vector<std::vector<int>>& flushes, int& currJokerValue, int& jokerSuit,
                       int& jokerHandStrength, int& straightFlush, int& flush, int& flushIndex) {

  for (int j = 0; j < 4; j++) {
    maxFlushSizeIndex = flushes[j].size() > flushes[maxFlushSizeIndex].size() ? j : maxFlushSizeIndex;
    if (flushes[j].size() > 3 && jokerEval) {
      if (flushes[j][flushes[j].size() - 1] - flushes[j][flushes[j].size() - 4] == 4) {
        currJokerValue = findMissing(flushes[j]);
        straightFlush = flushes[j][flushes[j].size() - 1];
        jokerSuit = j;
        jokerHandStrength = 8;

      } else if (flushes[j][flushes[j].size() - 1] - flushes[j][flushes[j].size() - 4] == 3) {
        currJokerValue = flushes[j][flushes[j].size() - 1] == 14 ? 10 : flushes[j][flushes[j].size() - 1] + 1;
        straightFlush = std::max(currJokerValue, flushes[j].back());
        jokerSuit = j;
        jokerHandStrength = 8;
      }
    }
    if (flushes[j].size() > 4) {
      if (flushes[j][flushes[j].size() - 1] - flushes[j][flushes[j].size() - 5] == 4) {
        straightFlush = flushes[j][flushes[j].size() - 1];
      }

      if (flushIndex == -1) {
        flushIndex = j;
      } else {
        for (int k = 0; k < 5; k++) {
          if (flushes[j][flushes[j].size() - 1 - k] > flushes[flushIndex][flushes[flushIndex].size() - 1 - k]) {
            flushIndex = j;
          } else if (flushes[j][flushes[j].size() - 1 - k] < flushes[flushIndex][flushes[flushIndex].size() - 1 - k]) {
            break;
          }
        }
      }
      flush = flushes[flushIndex][flushes[flushIndex].size() - 1];
      Utilities::Debug("Flush: " + std::to_string(flush));
    }
  }
}

void Player::NormieFlush(int jokerEval, int& jokerHandStrength, int& jokerSuit, int& flush, int& flushIndex, int& currJokerValue,
                         int& maxFlushSizeIndex, std::vector<std::vector<int>>& flushes) {
  if (jokerEval && jokerHandStrength < 5 && flushes[maxFlushSizeIndex].size() > 3) {
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
}

void Player::checkCountHands(int count, int countIndex, int& pair, int& twoPair, int& trips, int& straight_, int& flush, int& fullHouse,
                             int& quads, int& straightFlush, int& quint, std::priority_queue<int, std::vector<int>>& highCards,
                             int& currJokerValue, int& jokerHandStrength, int jokerEval) {

  Utilities::Debug("Checking Count Hands");
  if (jokerEval) {
    Utilities::Debug("Joker Eval");
  }
  Utilities::Debug("Count: " + std::to_string(count));

  int tempCurrJokerValue;
  int tempJokerHandStrength;
  if (!jokerEval) {
    tempCurrJokerValue = currJokerValue;
    tempJokerHandStrength = jokerHandStrength;
    jokerHandStrength = -1;
  }
  switch (count) {
    case 5:
      quint = countIndex + 1;
      currJokerValue = quint;
      jokerHandStrength = 9;
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
      if (jokerEval && jokerHandStrength < 1) {
        highCards.push(14);
        currJokerValue = 14;
        jokerHandStrength = 0;
      } else {
        highCards.push(countIndex + 1);
      }
      break;

    default:
      break;
  }

  if (!jokerEval) {
    currJokerValue = tempCurrJokerValue;
    jokerHandStrength = tempJokerHandStrength;
  }
}