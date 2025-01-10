#ifndef TEXTFORMAT_HPP
#define TEXTFORMAT_HPP
#include <unordered_map>
#include <filesystem>
#include <string>

using namespace std::filesystem;
using namespace std;

enum FormatType {
  None,
  Simple,
  NerdFont,
};

string formatText(const path&, FormatType);

#endif
