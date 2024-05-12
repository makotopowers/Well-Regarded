

#include <csignal>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#include "configReader.hpp"
#include "experiments.hpp"
#include "gameHandler.hpp"
#include "player.hpp"
#include "utilities.hpp"

std::atomic<bool> segFault = false;

void segFaultHandler(int sig) {
  std::cout << "SEGFAULT CAUGHT" << std::endl;
  segFault = true;

  exit(1);
}

int main() {
  std::string intro =
      "##  ##    ## ##   ##  ###             ##     ### ##   ### ###  \n"
      "##  ##   ##   ##  ##   ##              ##     ##  ##   ##  ##    \n"
      "##  ##   ##   ##  ##   ##            ## ##    ##  ##   ##         \n"
      " ## ##   ##   ##  ##   ##            ##  ##   ## ##    ## ##     \n"
      "  ##     ##   ##  ##   ##            ## ###   ## ##    ##       \n"
      "  ##     ##   ##  ##   ##            ##  ##   ##  ##   ##  ##    \n"
      "  ##      ## ##    ## ##            ###  ##  #### ##  ### ###    \n"
      "\n"
      "##   ##  ### ###  ####     ####              ### ##   ### ###   ## ##   "
      "  ##     ### ##   ### ##   ### ###  ### ##  \n"
      "##   ##   ##  ##   ##       ##                ##  ##   ##  ##  ##   ##  "
      "   ##     ##  ##   ##  ##   ##  ##   ##  ##  \n"
      "##   ##   ##       ##       ##                ##  ##   ##      ##       "
      " ## ##    ##  ##   ##  ##   ##       ##  ##  \n"
      "## # ##   ## ##    ##       ##                ## ##    ## ##   ##  ###  "
      " ##  ##   ## ##    ##  ##   ## ##    ##  ##  \n"
      "# ### #   ##       ##       ##                ## ##    ##      ##   ##  "
      " ## ###   ## ##    ##  ##   ##       ##  ##  \n"
      " ## ##    ##  ##   ##  ##   ##  ##            ##  ##   ##  ##  ##   ##  "
      " ##  ##   ##  ##   ##  ##   ##  ##   ##  ##  \n"
      "##   ##  ### ###  ### ###  ### ###           #### ##  ### ###   ## ##   "
      "###  ##  #### ##  ### ##   ### ###  ### ##   \n";

  std::cout << intro << std::endl;
  std::cout << std::endl;

  std::signal(SIGSEGV, segFaultHandler);

  std::string confDir = "../conf/";
  std::string confFile = "global.conf";
  ConfigReader configReader;
  configReader.readConfigFile(confDir + confFile);

  Utilities utilities;
  utilities.setDebug(configReader.returnConfigValue<int>("DEBUG"));
  Utilities::Debug("Debug mode enabled");
  Utilities::Debug(Utilities::debug ? "true" : "false");

  if (configReader.returnConfigValue<int>("PLAY_GAME") == 1) {
    Utilities::Debug("Playing game");
    Player* player1 = new Player("Makoto");
    Player* player2 = new Player("Rishi");
    std::unique_ptr<Player> player1Ptr(player1);
    std::unique_ptr<Player> player2Ptr(player2);
    GameHandler gameHandler(player1Ptr, player2Ptr);
    gameHandler.startGame();
    if (segFault) {
      gameHandler.~GameHandler();
    }
  }

  if (configReader.returnConfigValue<int>("EXPERIMENTS") == 1) {
    Utilities::Log("Running Experiments");
    Experiments experiments;
    std::unique_ptr<Player> player1 = std::make_unique<Player>("Makoto");
    std::unique_ptr<Player> player2 = std::make_unique<Player>("Rishi");
    std::vector<int> player1PlayOrder;
    std::vector<int> player2PlayOrder;
    int player1Tricks = configReader.returnConfigValue<int>("PLAYER1_TRICKS");
    int player2Tricks = configReader.returnConfigValue<int>("PLAYER2_TRICKS");
    int player1Jokers = configReader.returnConfigValue<int>("PLAYER1_JOKERS");
    int player2Jokers = configReader.returnConfigValue<int>("PLAYER2_JOKERS");
    int player1NumCardsLeft = configReader.returnConfigValue<int>("PLAYER1_NUM_CARDS_LEFT");
    int player2NumCardsLeft = configReader.returnConfigValue<int>("PLAYER2_NUM_CARDS_LEFT");

    std::istringstream p1PlayOrderStr(configReader.returnConfigValue<std::string>("PLAYER1_PLAY_ORDER"));
    std::istringstream p2PlayOrderStr(configReader.returnConfigValue<std::string>("PLAYER2_PLAY_ORDER"));
    std::string token;
    while (std::getline(p1PlayOrderStr, token, '_')) {
      Utilities::Debug("Token: " + token);
      player1PlayOrder.push_back(std::stoi(token));
    }
    while (std::getline(p2PlayOrderStr, token, '_')) {
      Utilities::Debug("Token: " + token);
      player2PlayOrder.push_back(std::stoi(token));
    }

    player1->setState(player1Tricks, player1PlayOrder, player1Jokers, player1NumCardsLeft);
    player2->setState(player2Tricks, player2PlayOrder, player2Jokers, player2NumCardsLeft);
    player1->evaluateHand();
    player2->evaluateHand();

    std::vector<double> a = experiments.spotTestp(player1, player2, configReader.returnConfigValue<int>("E_ITERATIONS"),
                                                  configReader.returnConfigValue<int>("E_VISUAL"));

    Utilities::Log("Mean: " + std::to_string(a[0]));
    Utilities::Log("Variance: " + std::to_string(a[1]));
  }

  return 0;
}
