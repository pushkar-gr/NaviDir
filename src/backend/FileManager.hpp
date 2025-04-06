#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP
#include "../config/Config.hpp"
#include "../frontend/TextFormat.hpp"
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std::filesystem;
using namespace std;

class FileManager {
private:
  Config *config;                       // pointer to config
  path currentPath;                     // path of current directory
  vector<directory_entry> currentFiles; // vector of files in current directory
  vector<directory_entry *>
      currentFilesFiltered; // vector of files in current directory with filter
                            // applied
  vector<string> currentFilesString;

  directory_entry selectedFile; // user selected file
  int selectedIndex;            // index of selectedPath

  vector<directory_entry>
      selectedFileChildren; // vector of children of selected files (if selected
                            // file is a directory)
  vector<directory_entry *>
      selectedFileChildrenFiltered; // vector of children of selected files (if
                                    // selected file is a directory)
  string selectedFileContent; // content of selected file(if selected file is
                              // not a directory)
  string selectedFileDisplayContent; // contents selectedFileContent if selected
                                     // file is not a directory, else contains
                                     // childrend directory in a list form

  bool updateFiles(vector<directory_entry> &,
                   const directory_entry &); // fills the vector with files from
                                             // the specified directory
  bool updateFiles(
      vector<directory_entry> &,
      const path &); // fills the vector with files from the specified path

  bool isLikelyTextFile(
      const std::filesystem::path &,
      size_t bytesToCheck = 512); // checks if file is text file or binary file

  void
  updateSelectedData(); // updates selectedFilesChildren or selectedFileContent
                        // depending on selected file type

public:
  FileManager(const directory_entry &,
              Config *); // constructor, initializes at given directory
  FileManager(Config *); // constructor, initializes at current directory
  FileManager(const path &, Config *); // constructor, initializes at given path
  FileManager(const FileManager &); // constructor, initilizes from given object

  const path *getCurrentPath() const; // gets the current directory program is
                                      // in

  vector<directory_entry *>::const_iterator filesBegin()
      const; // begin of const iterator to read files in current directory
  vector<directory_entry *>::const_iterator
  filesEnd() const; // end of const iterator to read files in current directory
  const vector<string> *
  getCurrentFilesString() const; // gets pointer to vector of current file names
  int *getSelectedIndex();       // gets pointer to selectedIndex
  const directory_entry &getSelectedFile() const; // gets selected file

  bool selectFile(const directory_entry &,
                  bool skipCheck = false); // selects the given file
  bool selectFile(const path &,
                  bool skipCheck = false); // selelcts the given file
  bool selectFile();    // updates the selected file based on index
  bool selectFile(int); // updates the index and selected file
  bool incrementSelected(
      bool cycle = true); // selects next file in the current directory
  bool decrementSelected(
      bool cycle = true); // selects previous file in the current directory
  bool selectParentDir(bool overRide = false); // selects the parent directory

  bool
  isSelectedDirectory() const; // returns true if selected file is a directory

  vector<directory_entry *>::const_iterator selectedFilesBegin()
      const; // begin of const iterator to read files in selected file
  vector<directory_entry *>::const_iterator selectedFilesEnd()
      const; // end of const iterator to read files in selected file
  const string &
  getSelectedFileContent() const; // returns content of selected file
  const string *getSelectedDisplayContent()
      const; // returns reference to selectedFileDisplayContent

  const path &switchPath(const directory_entry,
                         bool skipCheck = false); // switchs to given directory
  const path &switchPath(path &,
                         bool skipCheck = false); // switchs to given path
  const path &switchPath();     // switches to selected directory (selectedPath)
  const path &switchToParent(); // switches to parent directory

  bool refresh(); // refreshes current and selected data
  bool refreshCurrent();
  bool refreshSelected();

  bool createFile(const path &, string *);      // creates new file
  bool renameSelected(const path &, string *);  // renames selected file
  bool deleteSelected(string *);                // delets selected file
  bool pasteCopiedFile(const path &, string *); // paste copied file
  bool pasteCutFile(const path &, string *);    // paste cut file

  ~FileManager(); // destructor, does nothing as of now
};

#endif
