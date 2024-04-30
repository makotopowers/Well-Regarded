#ifndef CONFIG_READER_HPP
#define CONFIG_READER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class ConfigReader {
 private:
 public:
  vector<string> vec;

  void readConfigFile(string fname);

  string selectConfigEntry(string entr) const;
  template <typename returnValue>
  returnValue returnConfigValue(string entr) const {
    string str = selectConfigEntry(entr);
    stringstream ss(str);
    returnValue convertedValue;
    if (ss >> convertedValue) {
      return convertedValue;
    } else {
      throw std::runtime_error("Error: Could not convert config value to desired type.");
    }
  }
};

#endif  // CONFIG_READER_HPP