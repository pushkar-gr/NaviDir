#ifndef TEXTFORMAT_HPP
#define TEXTFORMAT_HPP
#include "../config/Config.hpp"
#include <ctime>
#include <filesystem>
#include <string>
#include <unordered_map>

using namespace std::filesystem;
using namespace std;

enum FormatType {
  None,     // NOne format type
  Simple,   // simple format type, will add an '/' at end if file is a directory
  NerdFont, // will add respective icon at start
};

string formatText(const path &, FormatType, bool fileName = true);
string formatText(const directory_entry &, FormatType, bool fileName = true);
string formatText(const path &, FormatType, Config *, bool fileName = true);
string formatText(const directory_entry &, FormatType, Config *,
                  bool fileName = true);

#endif
