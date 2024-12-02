#include "file_manager.h"
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <string>
#include <variant>
#include <vector>

using namespace std::filesystem;
using namespace std;

void FileManager::updateChildren(vector<directory_entry> *vec, path path) {
  vec->clear();
  for (const auto& entry : directory_iterator(path)) {
    vec->push_back(entry);
  }
}

void FileManager::updateSelectedData() {
  directory_entry selectedEntry (selectedPath);
  if (selectedEntry.is_directory()) {
    if (!holds_alternative<vector<directory_entry>>(selectedData)) {
      selectedData = vector<directory_entry>();
    }
    updateChildren(&get<vector<directory_entry>>(selectedData), selectedPath);
  } else {
    ifstream file (selectedPath);
    if (!file) {
      selectedData = "Unable to open file!";
      return;
    }
    if (!holds_alternative<string>(selectedData)) {
      selectedData = string();
    }
    string line;
    string content = get<string>(selectedData);
    content.clear();
    while (getline(file, line)) {
      content += line + "\n";
    }
    selectedData = content;
  }
}

FileManager::FileManager() {
  FileManager(path(current_path()));
}

FileManager::FileManager(path path) {
  switchPath(path);
}

path FileManager::getCurrentPath() {
  return currentPath;
}

vector<directory_entry> FileManager::getChildren() {
  return children;
}

directory_entry FileManager::getSelectedItem() {
  return directory_entry(selectedPath);
}

path FileManager::getSelectedPath() {
  return selectedPath;
}

bool FileManager::selectPath(path path, bool skipCheck=false) {
  if (! skipCheck) {
    for (int i = 0; i < children.size(); i++) {
      if (children[i] == path) {
        return selectPath(i);
      }
    }
  }
  if (path == selectedPath || ! exists(path)) {
    return false;
  }
  selectedPath = path;
  selected = -1;
  return true;
}

bool FileManager::selectPath() {
  if (selected < 0 || selected >= children.size()) {
    return false;
  }
  selectedPath = children[selected].path();
  updateSelectedData();
  return true;
}

bool FileManager::selectPath(int index) {
  if (selected == -1) {
    selected = 0;
  }
  int oldIndex = selected;
  selected = index;
  if (! selectPath()) {
    selected = oldIndex;
    return false;
  }
  return true;
}

bool FileManager::incrementSelected(bool cycle=true) {
  if (selected == -1) {
    selected = 0;
  }
  if (selected + 1 >= children.size()) {
    return selectPath(0);
  }
  return selectPath(selected + 1);
}

bool FileManager::decrementSelected(bool cycle=true) {
  if (selected == -1) {
    selected = 0;
  }
  if (selected == 0) {
    if (children.size() == 0) {
      return false;
    }
    return selectPath(children.size() - 1);
  }
  return selectPath(selected - 1);
}

bool FileManager::selectParentDir() {
  if (selected == -1) {
    return selectedPath == currentPath.parent_path();
  }
  return selectPath(currentPath.parent_path(), true);
}

variant<vector<directory_entry>, string> FileManager::getSelectedData() {
  return selectedData;
}

path FileManager::switchPath(path path) {
  if (selectPath(path, true)) {
    return switchPath();
  }
  return currentPath;
}

path FileManager::switchPath() {
  currentPath = selectedPath;
  children.clear();
  int selectedIndex = -1;
  for (const auto& entry : directory_iterator(currentPath)) { //todo: use updateChildren function instead
    children.push_back(entry);
    if (entry.path() == selectedPath) {
      selectedIndex = children.size() - 1;
    }
  }

  selectedPath = NULL;
  selected = selectedIndex == -1 ? 0 : selectedIndex;
  selectPath();
  return currentPath;
}

path FileManager::switchToParent() {
  if (selectParentDir()) {
    return switchPath();
  }
  return currentPath;
}

FileManager::~FileManager() {}
