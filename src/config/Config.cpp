#include "Config.hpp"

Config::Config() {
  showHiddenFiles = false;
  showPerms = false;
  showFileSize = false;
  showDateMod = false;
  filter = "";
}

bool Config::toggleHiddenFiles() {
  showHiddenFiles = !showHiddenFiles;
  return showHiddenFiles;
}
bool Config::displayHiddenFiles() { return showHiddenFiles; }
bool Config::togglePerms() {
  showPerms = !showPerms;
  return showPerms;
}
bool Config::displayPerms() { return showPerms; }
bool Config::toggleFileSize() {
  showFileSize = !showFileSize;
  return showFileSize;
}
bool Config::displayFileSize() { return showFileSize; }
bool Config::toggleDateMod() {
  showDateMod = !showDateMod;
  return showDateMod;
}
bool Config::displayDateMod() { return showDateMod; }
string Config::setFilter(string input) {
  filter = input;
  return filter;
}
string Config::getFileter() { return filter; }

Config::~Config() {}
