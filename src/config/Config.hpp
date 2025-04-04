#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <string>

using namespace std;

class Config {
private:
  bool showHiddenFiles; // true => display hidden files
  bool showPerms;       // true => show file perms
  bool showFileSize;    // true => show file size
  bool showDateMod;     // true => show modification date
  string filter;        // apply filter for currentFiles

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
