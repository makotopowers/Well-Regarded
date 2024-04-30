#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <iostream>
#include <string>

class Utilities {
 public:
  Utilities();
  ~Utilities();

  static void Log(std::string message, bool debug = true);
  static int compareHand(std::vector<int> p1hand, std::vector<int> p2hand);
};

#endif  // UTILITIES_HPP