#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <iostream>
#include <string>

class Utilities {
 public:
  static int debug;

  Utilities();
  ~Utilities();

  static void Log(std::string message);
  static void Debug(std::string message);
  static int compareHand(std::vector<int> p1hand, std::vector<int> p2hand);
  void setDebug(int debug);
};

#endif  // UTILITIES_HPP