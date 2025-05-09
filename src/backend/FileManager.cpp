#include "FileManager.hpp"

bool FileManager::updateFiles(
    vector<directory_entry> &vec,
    const directory_entry &entry) { // clears the vector and fills it with files
                                    // from given directory
  vec.clear();
  if (!entry.is_directory()) { // if given file is not a directory
    selectedFileContent = "File is not a directory";
    selectedFileDisplayContent = selectedFileContent;
    return false;
  }
  try {
    for (const auto &entry : directory_iterator(entry)) {
      vec.push_back(entry);
    }
    return true; // return true for success
  } catch (const filesystem_error &e) {
    selectedFileContent = e.what();
    selectedFileDisplayContent = selectedFileContent;
    return false; // return false for error
  }
  return true;
}

bool FileManager::updateFiles(
    vector<directory_entry> &vec,
    const path &path) { // calls updateFiles(vector<directory_entry>&,
                        // directory_entry&) with directory_entry
  directory_entry entry(path);
  return updateFiles(vec, entry);
}

bool FileManager::isLikelyTextFile(
    const std::filesystem::path &path,
    size_t bytesToCheck) { // makes decision based on file extension, if likely
                           // a binary, reads file to confirm
  // heuristic based on file extension
  std::string extension = path.extension().string();
  std::transform(extension.begin(), extension.end(), extension.begin(),
                 ::tolower);
  if (extension == ".txt" || extension == ".md" || extension == ".cpp" ||
      extension == ".h" || extension == ".hpp" || extension == ".c" ||
      extension == ".log" || extension == ".js" || extension == ".py" ||
      extension == ".css" || extension == ".html" || extension == ".csv" ||
      extension == ".json" || extension == ".sh" || extension == ".conf" ||
      extension == ".ini" || extension == ".cmake" || extension == ".diff" ||
      extension == ".patch") {
    return true; // likely text based on extension
  }

  // heuristic based on content (checking for binary characters)
  std::ifstream file(path, std::ios::binary);
  if (!file) {
    return false; // unable to open, consider not text
  }
  char buffer[bytesToCheck];
  file.read(buffer, bytesToCheck);
  std::streamsize readCount = file.gcount();
  for (std::streamsize i = 0; i < readCount; ++i) {
    if (buffer[i] == 0) {
      return false; // found a null byte, likely binary
    }
    if (static_cast<unsigned char>(buffer[i]) < 32 && buffer[i] != '\n' &&
        buffer[i] != '\r' && buffer[i] != '\t') {
      return false; // found a less common control character, likely binary
    }
  }

  return true;
}

void FileManager::updateSelectedData() { // updates selectedFileChildren or
                                         // selectedFileContent depending on
                                         // selectedFile type
  if (selectedFile.is_directory()) {
    if (!updateFiles(selectedFileChildren,
                     (selectedFile.path()))) { // fills the vector with files if
                                               // selectedFile is a directory
      return; // return if error in reading files in directory
    }
    refreshSelected();
  } else { // writes the content of selectedFile to selectedFileContent if it is
           // not a directory
    if (!isLikelyTextFile(selectedFile.path())) {
      selectedFileContent = "[Not a recognized or likely text file]";
      selectedFileDisplayContent = selectedFileContent;
      return;
    }
    ifstream file(selectedFile.path());
    if (!file) {
      selectedFileContent = "Unable to open file!";
      selectedFileDisplayContent = selectedFileContent;
      return;
    }
    string content, line;
    int line_number = 0;
    while (++line_number < 100 && getline(file, line)) {
      content +=
          (line_number < 10 ? " " : "") + to_string(line_number) + " " + line;
      content.push_back(0xa);
    }
    selectedFileContent = content;
    selectedFileDisplayContent = content;
  }
}

FileManager::FileManager(
    const directory_entry &entry,
    Config *inputConfig) { // calls switchPath and initializes class in given
                           // directory
  config = inputConfig;
  switchPath(entry, true);
}

FileManager::FileManager(Config *inputConfig)
    : FileManager(current_path(), inputConfig) {
} // class constructor to initializes class in PWD

FileManager::FileManager(const path &path, Config *inputConfig)
    : FileManager(directory_entry(path), inputConfig) {
} // calls switchPath and initializes class in given path

FileManager::FileManager(const FileManager &fm)
    : FileManager(*fm.getCurrentPath(), fm.config) {}

const path *FileManager::getCurrentPath() const { // returns currentPath
  return &currentPath;
}

vector<directory_entry *>::const_iterator
FileManager::filesBegin() const { // returns cbegin of currentFilesFiltered
  return currentFilesFiltered.cbegin();
}

vector<directory_entry *>::const_iterator
FileManager::filesEnd() const { // returns cend of currentFilesFiltered
  return currentFilesFiltered.cend();
}

const vector<string> *FileManager::getCurrentFilesString()
    const { // returns pointer to currentFilesString
  return &currentFilesString;
}

int *FileManager::getSelectedIndex() { // return pointer to selectedIndex
  return &selectedIndex;
}

const directory_entry &
FileManager::getSelectedFile() const { // returns selectedFile
  return selectedFile;
}

bool FileManager::selectFile(const directory_entry &entry,
                             bool skipCheck) { // selects given entry
  if (!skipCheck) {
    for (int i = 0; i < currentFilesFiltered.size(); i++) {
      if (*(currentFilesFiltered)[i] == entry) {
        return selectFile(
            i); // calls selectFile(int) if file found in currentFilesFiltered
      }
    }
  }
  if (selectedFile == entry || !entry.exists()) {
    return false; // returns false if file does not exist or file already
                  // selected
  }
  // file not found in currentFilesFiltered
  selectedFile = entry;
  selectedIndex = -1;
  updateSelectedData();
  return true;
}

bool FileManager::selectFile(
    const path &path,
    bool skipCheck) { // calls selectFile(directory_entry&, bool)
  directory_entry entry(path);
  return selectFile(entry, skipCheck);
}

bool FileManager::selectFile() { // calls selectedFile(int)
  if (selectedIndex == -1) {
    selectedIndex = 0;
  }
  return selectFile(selectedIndex);
}

bool FileManager::selectFile(int index) { // will select the file at index
  if (index < 0 || index >= currentFilesFiltered.size()) {
    if (currentFilesFiltered.size() == 0) {
      return false; // no files available
    }
    index = currentFilesFiltered.size() - 1;
  }
  selectedIndex = index;
  selectedFile = *(currentFilesFiltered)[index];
  updateSelectedData();
  return true;
}

bool FileManager::incrementSelected(bool cycle) { // selects index + 1
  if (selectedIndex == -1) {
    selectedIndex = 0;
  }
  if (selectedIndex + 1 >= currentFilesFiltered.size()) {
    return cycle && selectFile(0); // selects 0th element if cycle is true and
                                   // index = size() - 1
  }
  return selectFile(selectedIndex + 1);
}

bool FileManager::decrementSelected(bool cycle) { // selects index - 1
  if (selectedIndex == -1) {
    selectedIndex = 0;
  }
  if (selectedIndex == 0) {
    return cycle &&
           selectFile(
               currentFilesFiltered.size() -
               1); // selects (n-1)th index if cycle is true and index = 0
  }
  return selectFile(selectedIndex - 1);
}

bool FileManager::selectParentDir(
    bool overRide) { // selects parent directory of currentPath
  if (!overRide && selectedIndex == -1) { // skips if overRide is true
    return selectedFile.path() ==
           currentPath
               .parent_path(); // returns if already selected parent directory
  }
  path path = currentPath.parent_path();
  return selectFile(path, true);
}

bool FileManager::isSelectedDirectory()
    const { // returns true if selectedFile is a directory
  return selectedFile.is_directory();
}

vector<directory_entry *>::const_iterator FileManager::selectedFilesBegin()
    const { // return cbegin of selectedFileChildrenFiltered if selectedFile is
            // a directory
  if (!selectedFile.is_directory()) {
    return selectedFileChildrenFiltered.cend();
  }
  return selectedFileChildrenFiltered.cbegin();
}

vector<directory_entry *>::const_iterator FileManager::selectedFilesEnd()
    const { // returns cend of selectedFileChildrenFiltered
  return selectedFileChildrenFiltered.cend();
}

const string &FileManager::getSelectedFileContent()
    const { // returns content of selectedFile if it is not a directory
  if (selectedFile.is_directory()) {
    return NULL;
  }
  return selectedFileContent;
}

const string *FileManager::getSelectedDisplayContent()
    const { // return a pointer to selectedFileDisplayContent
  return &selectedFileDisplayContent;
}

const path &
FileManager::switchPath(const directory_entry entry,
                        bool skipCheck) { // changes currentPath to give path
  if (!entry.is_directory()) {
    return currentPath; // return if given path is not a directory
  }
  selectedFile = directory_entry(currentPath);
  currentPath = entry.path();
  selectedIndex = 0;
  if (!skipCheck) {
    currentFiles.clear();
    for (const auto &dir_entry : directory_iterator(currentPath)) {
      currentFiles.push_back(dir_entry);
      if (dir_entry == selectedFile) {
        selectedIndex = currentFiles.size() -
                        1; // previous currentPath found in new directory
      }
    }
  } else {
    updateFiles(currentFiles, currentPath);
  }
  refreshCurrent();
  selectFile();
  return currentPath;
}

const path &FileManager::switchPath(
    path &path, bool skipCheck) { // calls switchPath(directory_entry&, bool)
  return switchPath(directory_entry(path), skipCheck);
}

const path &
FileManager::switchPath() { // calls switchPath(directory_entry&, bool)
  return switchPath(selectedFile, true);
}

const path &
FileManager::switchToParent() { // calls switchPath(directory_entry&, bool)
  directory_entry entry(currentPath.parent_path());
  return switchPath(entry, false);
}

bool FileManager::refresh() { // refreshes data
  if (!updateFiles(currentFiles, currentPath)) {
    return false;
  }
  refreshCurrent();
  refreshSelected();
  selectFile();
  updateSelectedData();
  return true;
}

bool isEqual(string s1, string s2) {
  if (s1.size() > s2.size()) {
    return false;
  }
  for (int i = 0; i < s1.size(); i++) {
    if (tolower(s1.at(i)) != tolower(s2.at(i))) {
      return false;
    }
  }
  return true;
}

bool FileManager::refreshCurrent() {
  currentFilesFiltered.clear();
  currentFilesString.clear();
  for (directory_entry &entry : currentFiles) {
    if (config->getFileter().size() != 0) {
      if (isEqual(config->getFileter(), entry.path().filename())) {
        currentFilesFiltered.push_back(&entry);
        currentFilesString.push_back(
            formatText(entry.path(), FormatType::NerdFont, config));
        if (selectedFile == entry) {
          selectedIndex = currentFilesFiltered.size() - 1;
        }
      }
    } else if (config->displayHiddenFiles() ||
               entry.path().filename().string().at(0) != '.') {
      currentFilesFiltered.push_back(&entry);
      currentFilesString.push_back(
          formatText(entry.path(), FormatType::NerdFont, config));
      if (selectedFile == entry) {
        selectedIndex = currentFilesFiltered.size() - 1;
      }
    }
  }
  return true;
}

bool FileManager::refreshSelected() {
  if (!selectedFile.is_directory()) {
    return false;
  }
  selectedFileDisplayContent = "";
  selectedFileChildrenFiltered.clear();
  for (directory_entry &entry : selectedFileChildren) {
    if (config->displayHiddenFiles() ||
        entry.path().filename().string().at(0) != '.') {
      selectedFileChildrenFiltered.push_back(&entry);
      selectedFileDisplayContent +=
          formatText(entry.path(), FormatType::NerdFont, config);
      selectedFileDisplayContent.push_back(0xa);
    }
  }
  return true;
}

bool FileManager::createFile(const path &path, string *output) {
  if (exists(path)) {
    *output = "File already exists";
    return false;
  }
  string pathString = path.string();
  char lastChar = pathString.at(pathString.size() - 1);
  try {
    if (lastChar == '/' || lastChar == '\\') {
      create_directory(path);
      pathString.pop_back();
    } else {
      ofstream file(pathString);
      if (!exists(pathString)) {
        *output = "Unexpected error occured";
        return false;
      }
    }
    refresh();
    return selectFile(pathString);
  } catch (const exception &e) {
    *output = e.what();
    return false;
  }
}

bool FileManager::renameSelected(const path &path, string *output) {
  if (exists(path)) {
    *output = "File already exists";
    return false;
  }
  try {
    rename(selectedFile, path);
    return refresh();
  } catch (const filesystem_error &e) {
    *output = e.what();
    return false;
  }
}

bool FileManager::deleteSelected(string *output) {
  if (!selectedFile.exists()) {
    *output = "File does not exists";
    refresh();
    return false;
  }
  try {
    int index = selectedIndex;
    remove_all(selectedFile);
    refresh();
    switchPath(currentPath);
    return true;
    if (currentFiles.size() == 0) {
      selectedFileDisplayContent = "";
    } else {
      selectFile(index);
    }
  } catch (const filesystem_error &e) {
    *output = e.what();
    return false;
  }
}

bool FileManager::pasteCopiedFile(const path &path, string *output) {
  class path newPath = currentPath / path.filename();
  if (exists(newPath)) {
    *output = "File already exists";
    return false;
  }
  try {
    copy(path, newPath);
    refresh();
    return selectFile(newPath);
  } catch (const filesystem_error &e) {
    *output = e.what();
    return false;
  }
}

bool FileManager::pasteCutFile(const path &path, string *output) {
  class path newPath = currentPath / path.filename();
  if (exists(newPath)) {
    *output = "File already exists";
    return false;
  }
  try {
    rename(path, newPath);
    refresh();
    return selectFile(newPath);
  } catch (const filesystem_error &e) {
    *output = e.what();
    return false;
  }
}

FileManager::~FileManager() {} // destructor
