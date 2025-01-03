#include "FileManager.hpp"

void FileManager::updateFiles(vector<directory_entry>& vec, const directory_entry& entry) { //clears the vector and fills it with files from given directory
  vec.clear();
  if (! entry.is_directory()) {
    return;
  }
  for (const auto& entry : directory_iterator(entry)) {
    vec.push_back(entry);
  }
}

void FileManager::updateFiles(vector<directory_entry>& vec, const path& path) { //calls updateFiles(vector<directory_entry>&, directory_entry&) with directory_entry
  directory_entry entry(path);
  updateFiles(vec, entry);
}

void FileManager::updateSelectedData() { //updates selectedFileChildren or selectedFileContent depending on selectedFile type
  if (selectedFile.is_directory()) {
    updateFiles(selectedFileChildren, (selectedFile.path())); //fills the vector with files if selectedFile is a directory
    applyFilterSelected(NONE);
  } else { //writes the content of selectedFile to selectedFileContent if it is not a directory
    ifstream file (selectedFile.path());
    if (!file) {
      selectedFileContent = "Unable to open file!";
      return;
    }
    string content, line;
    while (getline(file, line)) {
      content += line + "\n";
    }
    selectedFileContent = content;
  }
}

bool FileManager::applyNoneFilterCurrent() { //fills the filter vector with pointer to all the files
  currentFilesFiltered.clear();
  currentFilesString.clear();
  for (directory_entry &entry : currentFiles) {
    currentFilesFiltered.push_back(&entry);
    currentFilesString.push_back(entry.path().string()); //todo: call formatString from forntend to add any icon/color to the string
  }
  return true;
}

bool FileManager::applyNoneFilterSelected() { //fills the filter vector with pointer to all the files
  if (! selectedFile.is_directory()) {
    return false;
  }
  selectedFileChildrenFiltered.clear();
  for (directory_entry &entry : selectedFileChildren) {
    selectedFileChildrenFiltered.push_back(&entry);
  }
  return true;
}

FileManager::FileManager(const directory_entry& entry) { //calls switchPath and initializes class in given directory
  switchPath(entry, true);
}

FileManager::FileManager() : FileManager(current_path()) {} //class constructor to initializes class in PWD

FileManager::FileManager(const path& path) : FileManager(directory_entry(path)) {} //calls switchPath and initializes class in given path

FileManager::FileManager(const FileManager& fm) : FileManager(fm.getCurrentPath()) {} 

const path& FileManager::getCurrentPath() const { //returns currentPath
  return currentPath;
}

vector<directory_entry*>::const_iterator FileManager::filesBegin() const { //returns cbegin of currentFilesFiltered
  return currentFilesFiltered.cbegin();
}

vector<directory_entry*>::const_iterator FileManager::filesEnd() const { //returns cend of currentFilesFiltered
  return currentFilesFiltered.cend();
}

const vector<string> *FileManager::getCurrentFilesString() const { //returns pointer to currentFilesString
  return &currentFilesString;
}

const int *FileManager::getSelectedIndex() const {
  return &selectedIndex;
}

const directory_entry& FileManager::getSelectedFile() const { //returns selectedFile
  return selectedFile;
}

bool FileManager::selectFile(directory_entry& entry, bool skipCheck) { //selects given entry
  if (! skipCheck) {
    for (int i = 0; i < currentFilesFiltered.size(); i++) {
      if (*(currentFilesFiltered)[i] == entry) {
        return selectFile(i); //calls selectFile(int) if file found in currentFilesFiltered
      }
    }
  }
  if (selectedFile == entry || ! entry.exists()) {
    return false; //returns false if file does not exist or file already selected
  }
  //file not found in currentFilesFiltered
  selectedFile = entry;
  selectedIndex = -1;
  updateSelectedData();
  return true;
}

bool FileManager::selectFile(path& path, bool skipCheck) { //calls selectFile(directory_entry&, bool)
  directory_entry entry(path);
  return selectFile(entry, skipCheck);
}

bool FileManager::selectFile() { //calls selectedFile(int)
  if (selectedIndex == -1) {
    selectedIndex = 0;
  }
  return selectFile(selectedIndex);
}

bool FileManager::selectFile(int index) { //will select the file at index
  if (index < 0 || index >= currentFilesFiltered.size()) {
    return false; //returns false if index out of range
  }
  selectedIndex = index;
  selectedFile = *(currentFilesFiltered)[index];
  updateSelectedData();
  return true;
}

bool FileManager::incrementSelected(bool cycle) { //selects index + 1
  if (selectedIndex == -1) {
    selectedIndex = 0;
  }
  if (selectedIndex + 1 >= currentFilesFiltered.size()) {
    return cycle && selectFile(0); //selects 0th element if cycle is true and index = size() - 1
  }
  return selectFile(selectedIndex + 1);
}

bool FileManager::decrementSelected(bool cycle) { //selects index - 1
  if (selectedIndex == -1) {
    selectedIndex = 0;
  }
  if (selectedIndex == 0) {
    return cycle && selectFile(currentFilesFiltered.size() - 1); //selects (n-1)th index if cycle is true and index = 0
  }
  return selectFile(selectedIndex - 1);
}

bool FileManager::selectParentDir(bool overRide) { //selects parent directory of currentPath
  if (! overRide && selectedIndex == -1) { //skips if overRide is true
    return selectedFile.path() == currentPath.parent_path(); //returns if already selected parent directory
  }
  path path = currentPath.parent_path();
  return selectFile(path, true);
}

bool FileManager::isSelectedDirectory() const { //returns true if selectedFile is a directory
  return selectedFile.is_directory();
}

vector<directory_entry*>::const_iterator FileManager::selectedFilesBegin() const { //return cbegin of selectedFileChildrenFiltered if selectedFile is a directory
  if (! selectedFile.is_directory()) {
    return selectedFileChildrenFiltered.cend();
  }
  return selectedFileChildrenFiltered.cbegin();
}

vector<directory_entry*>::const_iterator FileManager::selectedFilesEnd() const { //returns cend of selectedFileChildrenFiltered
  return selectedFileChildrenFiltered.cend();
}

const string& FileManager::getSelectedFileContent() { //returns content of selectedFile if it is not a directory
  if (selectedFile.is_directory()) {
    return NULL;
  }
  return selectedFileContent;
}

const path& FileManager::switchPath(const directory_entry entry, bool skipCheck) { //changes currentPath to give path
  if (! entry.is_directory()) {
    return currentPath; //return if given path is not a directory
  }
  selectedFile = directory_entry(currentPath);
  currentPath = entry.path();
  selectedIndex = 0;
  if (! skipCheck) {
    currentFiles.clear();
    for (const auto& dir_entry : directory_iterator(currentPath)) {
      currentFiles.push_back(entry);
      if (dir_entry == selectedFile) {
        selectedIndex = currentFiles.size() - 1; //previous currentPath found in new directory
      }
    }
  } else {
    updateFiles(currentFiles, currentPath);
  }
  applyFilterCurrent(NONE);
  selectFile();
  return currentPath;
}

const path& FileManager::switchPath(path& path, bool skipCheck) { //calls switchPath(directory_entry&, bool)
  return switchPath(directory_entry(path), skipCheck);
}

const path& FileManager::switchPath() { //calls switchPath(directory_entry&, bool)
  return switchPath(selectedFile, true);
}

const path& FileManager::switchToParent() { //calls switchPath(directory_entry&, bool)
  directory_entry entry(currentPath.parent_path());
  return switchPath(entry, false);
}

bool FileManager::applyFilter(FilterType type) { //applies filter to both current and selected 
  return applyFilterCurrent(type) && applyFilterSelected(type);
}

bool FileManager::applyFilterCurrent(FilterType type) { //calls respective filters
  if (type == NONE) {
    return applyNoneFilterCurrent();
  }
  return false;
}

bool FileManager::applyFilterSelected(FilterType type) { //calls respective filters
  if (type == NONE) {
    return applyNoneFilterSelected();
  }
  return false;
}

FileManager::~FileManager() {} //destructor
