#include "configReader.hpp"
#include <fstream>

using namespace std;

void ConfigReader::readConfigFile(string fname) {
  try {
    vec.clear();
    ifstream inFile;
    inFile.open(fname);

    string str;
    while (getline(inFile, str)) {
      vec.push_back(str);
    }
    inFile.close();
  } catch (std::exception const& e) {
    std::cerr << e.what() << '\n';
  }
}

string ConfigReader::selectConfigEntry(string entr) const {
  string sub;
  for (int i = 0; i < (int)vec.size(); i++) {
    sub = vec[i].substr(0, entr.size());
    if (sub == entr) {
      return vec[i].substr(entr.size() + 1, vec[i].size());
    }
  }
  return "0";
}