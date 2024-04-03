#include <gtest/gtest.h>

#include <player.hpp>

TEST(LaunchTest, Test1) { ASSERT_EQ(1, 1); }

TEST(HandEvalTest, Test2) {
  Player* player1 = new Player();
  Hand* hand = new Hand();
  player1->hand = hand;
  hand->cards[0][0] = 1;
  hand->cards[1][0] = 1;
  std::cout << "Hand created" << std::endl;

  player1->evaluateHand(hand);

  ASSERT_EQ(hand->pair, 1);
  delete hand;
  delete player1;
}

TEST(HandEvalTest, Test3) {
  Player* player1 = new Player();
  Hand* hand = new Hand();
  hand->cards[0][0] = 1;
  hand->cards[0][1] = 1;
  hand->cards[0][2] = 1;
  hand->cards[0][3] = 1;
  hand->cards[0][4] = 1;

  std::cout << "Hand created" << std::endl;

  player1->evaluateHand(hand);

  ASSERT_EQ(hand->straightFlush, 5);
  delete player1;
  delete hand;
}

TEST(HandEvalTest, Test4) {
  Player* player1 = new Player();
  Hand* hand = new Hand();
  hand->cards[0][0] = 1;
  hand->cards[1][0] = 1;
  hand->cards[2][6] = 1;
  hand->cards[1][6] = 1;
  hand->cards[0][6] = 1;

  std::cout << "Hand created" << std::endl;

  player1->evaluateHand(hand);

  ASSERT_EQ(hand->fullHouse, 7);
  ASSERT_EQ(hand->pair, 1);
  delete player1;
  delete hand;
}
TEST(HandEvalTest, Test5) {
  Player* player1 = new Player();
  Hand* hand = new Hand();
  hand->cards[0][0] = 1;
  hand->cards[1][0] = 1;

  hand->cards[1][4] = 1;
  hand->cards[1][7] = 1;
  hand->cards[0][5] = 1;
  hand->cards[2][6] = 1;
  hand->cards[3][8] = 1;
  std::cout << "Hand created" << std::endl;

  player1->evaluateHand(hand);

  ASSERT_EQ(hand->straight_, 9);
  delete player1;
  delete hand;
}

TEST(HandEvalTest, Test6) {
  Player* player1 = new Player();
  Hand* hand = new Hand();
  hand->cards[0][0] = 1;
  hand->cards[1][0] = 1;

  for (int i = 0; i < 1000000; i++) {
    player1->evaluateHand(hand);
  }

  ASSERT_EQ(hand->pair, 1);
  delete player1;
  delete hand;
}

TEST(AddCardTest, Test1) {
  Player* player1 = new Player();
  Hand* hand = new Hand();
  player1->hand = hand;
  player1->addCard(1);
  player1->addCard(2);
  player1->addCard(3);
  player1->addCard(4);
  player1->addCard(5);

  player1->evaluateHand(player1->hand);

  ASSERT_EQ(hand->straightFlush, 6);
  delete player1;
  delete hand;
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}