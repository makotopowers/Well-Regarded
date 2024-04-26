#include "gameHandler.hpp"

#include <iostream>
#include <string>

#include "player.hpp"

// #define debugPrint(x) std::cout << "[[DEBUG]] " << x << std::endl;
#define debugPrint(x) ;

GameHandler::GameHandler(std::unique_ptr<Player> player1_,
                         std::unique_ptr<Player> player2_)
    : player1(std::move(player1_)), player2(std::move(player2_)) {
  /// @brief Constructor for GameHandler
  /// @param player1_ std::unique_ptr<Player> for player1
  /// @param player2_ std::unique_ptr<Player> for player2

  this->Log("GameHandler constructor called");
}

GameHandler::~GameHandler() {
  /// @brief Destructor for GameHandler
  this->Log("GameHandler destructor called");
}

void GameHandler::startGame() {
  /// @brief Start the game

  this->Log("GameHandler startGame called");
  std::vector<int> p1hand;
  std::vector<int> p2hand;
  int tie = 1;
  int playerAhead = 0;
  int yield = 0;

  while (player1->cardsLeft.size() > 0 && player2->cardsLeft.size() > 0 &&
         player1->tricks < 5 && player2->tricks < 5) {
    if (tie == 1) {
      player1->playCard();
      player2->playCard();
      tie = 0;
    }
    this->Log(" ");
    p1hand = player1->evaluateHand(player1->hand);
    this->Log(player1->name + " hand: ");
    player1->printHand(player1->hand->value);
    p2hand = player2->evaluateHand(player2->hand);
    this->Log(player2->name + " hand: ");
    player2->printHand(player2->hand->value);

    playerAhead = compareHand(p1hand, p2hand);

    if (playerAhead == 1) {
      this->Log(player1->name + " is ahead");
    } else if (playerAhead == 2) {
      this->Log(player2->name + " is ahead");
    } else {
      this->Log("Tie");
    }

    if (playerAhead == 0) {
      tie = 1;
      continue;
    }

    if (playerAhead == 1) {
      yield = player2->turn(player1->hand, player1->tricks, player1->jokers,
                            player1->numCardsLeft);

    } else if (playerAhead == 2) {
      yield = player1->turn(player2->hand, player2->tricks, player2->jokers,
                            player2->numCardsLeft);
    }

    this->Log("Yield: " + std::to_string(yield));
    if (yield == 2) {
      tie = 1;
    } else if (yield == 1) {
      if (playerAhead == 1) {
        player1->tricks++;
      } else {
        player2->tricks++;
      }
      player1->resetHand();
      player2->resetHand();
      tie = 1;
    }
  }
}

int GameHandler::compareHand(std::vector<int> p1hand, std::vector<int> p2hand) {
  /// @brief Compare the hands of two players
  /// @param p1hand std::vector<int> representing the hand of player 1
  /// @param p2hand std::vector<int> representing the hand of player 2
  /// @return 1 if player 1 is ahead, 2 if player 2 is ahead, 0 if tie

  for (int i = 0; i < p1hand.size(); i++) {
    if (p1hand[i] > p2hand[i]) {
      return 1;
    } else if (p1hand[i] < p2hand[i]) {
      return 2;
    } else {
      continue;
    }
  }
  return 0;
}

void GameHandler::Log(std::string message, bool debug) {
  /// @brief Log a message
  /// @param message std::string message to log
  /// @param debug bool whether to log the message

  if (debug) {
    std::cout << message << std::endl;
  }
}