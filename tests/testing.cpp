#include <gtest/gtest.h>

#include <experiments.hpp>
#include <player.hpp>
#include <utilities.hpp>

/*
Quints = 9
Straight Flush = 8
Four of a Kind = 7
Full House = 6
Flush = 5
Straight = 4
Three of a Kind = 3
Two Pair = 2
One Pair = 1
High Card = 0
*/

// TEST(EvalTest, StraightFlushJokerEnd) {
//   Player* player1 = new Player("Makoto");
//   player1->addCard(1);
//   player1->printCard(1);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   player1->addCard(2);
//   player1->printCard(2);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   player1->addCard(3);
//   player1->printCard(3);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   player1->addCard(4);
//   player1->printCard(4);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   player1->addCard(52);
//   player1->printCard(52);
//   player1->printHand(player1->evaluateHand(player1->hand));

//   std::vector<int> ret = player1->evaluateHand(player1->hand);
//   std::vector<int> expected = {8, 6, 0, 0, 0, 0};
//   for (int i = 0; i < ret.size(); i++) {
//     std::cout << ret[i] << std::endl;
//     ASSERT_EQ(ret[i], expected[i]);
//   }
// }

// TEST(EvalTest, StraightFlushJokerEnd2) {
//   Player* player1 = new Player("Makoto");
//   player1->addCard(0);
//   player1->addCard(1);
//   player1->addCard(2);
//   player1->addCard(3);
//   player1->addCard(52);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   std::vector<int> ret = player1->evaluateHand(player1->hand);
//   std::vector<int> expected = {8, 5, 0, 0, 0, 0};
//   for (int i = 0; i < ret.size(); i++) {
//     std::cout << ret[i] << std::endl;
//     ASSERT_EQ(ret[i], expected[i]);
//   }
// }

// TEST(EvalTest, FullHousePairFirst) {
//   Player* player1 = new Player("Makoto");
//   player1->addCard(0);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   player1->addCard(13);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   player1->addCard(2);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   player1->addCard(15);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   player1->addCard(28);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   std::vector<int> ret = player1->evaluateHand(player1->hand);
//   std::vector<int> expected = {6, 3, 14, 0, 0, 0};
//   for (int i = 0; i < ret.size(); i++) {
//     // std::cout << ret[i] << std::endl;
//     ASSERT_EQ(ret[i], expected[i]);
//   }
// }
// TEST(EvalTest, FullHouseKings) {
//   Player* player1 = new Player("Makoto");
//   player1->addCard(12);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   player1->addCard(25);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   player1->addCard(2);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   player1->addCard(15);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   player1->addCard(28);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   std::vector<int> ret = player1->evaluateHand(player1->hand);
//   std::vector<int> expected = {6, 3, 13, 0, 0, 0};
//   for (int i = 0; i < ret.size(); i++) {
//     // std::cout << ret[i] << std::endl;
//     ASSERT_EQ(ret[i], expected[i]);
//   }
// }
// TEST(EvalTest, FullHouseJokerFirst) {
//   Player* player1 = new Player("Makoto");
//   player1->addCard(12);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   player1->addCard(52);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   player1->addCard(2);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   player1->addCard(15);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   player1->addCard(28);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   std::vector<int> ret = player1->evaluateHand(player1->hand);
//   std::vector<int> expected = {6, 3, 13, 0, 0, 0};
//   for (int i = 0; i < ret.size(); i++) {
//     // std::cout << ret[i] << std::endl;
//     ASSERT_EQ(ret[i], expected[i]);
//   }
// }
// TEST(EvalTest, Straight) {
//   Player* player1 = new Player("Makoto");
//   player1->addCard(1);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   player1->addCard(2);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   player1->addCard(3);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   player1->addCard(4);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   player1->addCard(18);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   std::vector<int> ret = player1->evaluateHand(player1->hand);
//   std::vector<int> expected = {4, 6, 0, 0, 0, 0};
//   for (int i = 0; i < ret.size(); i++) {
//     // std::cout << ret[i] << std::endl;
//     ASSERT_EQ(ret[i], expected[i]);
//   }
// }

// TEST(PrintCardTest, Test1) {
//   Player* player1 = new Player("Makoto");

//   player1->printCard(0);
//   player1->printCard(1);

//   player1->printCard(2);

//   player1->printCard(3);

//   player1->printCard(4);
//   player1->printCard(12);
//   player1->printCard(13);
//   player1->printCard(14);

//   player1->printCard(52);
// }

// TEST(TimingTest, Test1) {
//   Player* player1 = new Player("Makoto");
//   for (int i = 0; i < 50; i++) {
//     player1->addCard(i);
//   }

//   for (int i = 0; i < 100000; i++) {
//     player1->evaluateHand(player1->hand);
//   }
// }

// TEST(ResetTest, Test1) {
//   Player* player1 = new Player("Makoto");
//   for (int i = 0; i < 50; i++) {
//     player1->addCard(i);
//   }

//   player1->resetHand();
//   std::cout << player1->numCardsLeft << std::endl;
//   for (int i = 0; i < player1->playOrder.size(); i++) {
//     std::cout << player1->playOrder[i] << " ";
//   }
//   std::cout << std::endl;
// }

// TEST(ResetTest, Test2) {
//   Player* player1 = new Player("Makoto");
//   for (int i = 0; i < 50; i++) {
//     player1->addCard(i);
//   }

//   player1->resetPlayer();
//   std::cout << player1->numCardsLeft << std::endl;
//   for (int i = 0; i < player1->playOrder.size(); i++) {
//     std::cout << player1->playOrder[i] << " ";
//   }
//   std::cout << std::endl;
// }

// TEST(ResetTest, Test3) {
//   Player* player1 = new Player("Makoto");
//   for (int i = 0; i < 50; i++) {
//     player1->addCard(i);
//   }

//   player1->setState(3, {1, 2, 3, 4, 5});
//   std::cout << player1->numCardsLeft << std::endl;
//   std::cout << player1->tricks << std::endl;
//   for (int i = 0; i < player1->playOrder.size(); i++) {
//     std::cout << player1->playOrder[i] << " ";
//   }
// }

// TEST(ExperimentsTest, Test1) {
//   std::shared_ptr<Player> player1 = std::make_shared<Player>("Makoto");
//   std::shared_ptr<Player> player2 = std::make_shared<Player>("Kaito");
//   player1->addCard(0);
//   player1->addCard(13);
//   player2->addCard(1);
//   player1->printHand(player1->evaluateHand(player1->hand));
//   player2->printHand(player2->evaluateHand(player2->hand));
//   Experiments exp;
//   std::vector<int> ret = exp.spotTest(player1, player2, 1000);
//   std::cout << "Mean: " << ret[0] << std::endl;
//   std::cout << "Variance: " << ret[1] << std::endl;
//   ASSERT_EQ(ret.size(), 2);
// }

TEST(EvalTest, jjj) {
  Player* player1 = new Player("Makoto");
  std::vector<int> toAdd = {1, 17, 46, 9, 45, 25, 18, 48, 49, 53};
  for (int i = 0; i < toAdd.size(); i++) {
    player1->addCard(toAdd[i]);
    player1->evaluateHand(player1->hand);
  }
  std::vector<int> ret = player1->evaluateHand(player1->hand);
  std::vector<int> expected = {8, 11, 0, 0, 0, 0};
  for (int i = 0; i < ret.size(); i++) {
    // std::cout << ret[i] << std::endl;
    ASSERT_EQ(ret[i], expected[i]);
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}