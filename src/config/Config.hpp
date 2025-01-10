#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <string>

using namespace std;

class Config {
private:
  bool showHiddenFiles;
  bool showPerms;
  bool showFileSize;
  bool showDateMod;
  string filter;

public:
  Config();

  bool toggleHiddenFiles();
  bool displayHiddenFiles();
  bool togglePerms();
  bool displayPerms();
  bool toggleFileSize();
  bool displayFileSize();
  bool toggleDateMod();
  bool displayDateMod();
  string setFilter(string);
  string getFileter();

  ~Config();
};

#endif
