#ifndef TEXTFORMAT_HPP
#define TEXTFORMAT_HPP
#include <unordered_map>
#include <filesystem>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include "../config/Config.hpp"

using namespace std::filesystem;
using namespace std;

enum FormatType {
  None, //NOne format type
  Simple, //simple format type, will add an '/' at end if file is a directory
  NerdFont, //will add respective icon at start
};

string formatText(const path&, FormatType);
string formatText(const directory_entry&, FormatType);
string formatText(const path&, FormatType, Config *);
string formatText(const directory_entry&, FormatType, Config *);

#endif
