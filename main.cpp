

#include <csignal>
#include <iostream>
#include <string>

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

  Player *player1 = new Player("Makoto");
  Player *player2 = new Player("Rishi");
  GameHandler gameHandler(std::make_unique<Player>(*player1),
                          std::make_unique<Player>(*player2));
  gameHandler.startGame();

  if (segFault) {
    gameHandler.~GameHandler();
  }

  return 0;
}
