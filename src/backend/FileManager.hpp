#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP
#include <filesystem>
#include <vector>
#include <string>
#include <cstddef>
#include <cstdio>
#include <fstream>

using namespace std::filesystem;
using namespace std;

enum FilterType {
  NONE,
};

class FileManager {
private:
  path currentPath; //path of current directory
  vector<directory_entry> currentFiles; //vector of files in current directory
  vector<directory_entry*> currentFilesFiltered; //vector of files in current directory with filter applied
  vector<string> currentFilesString;

  directory_entry selectedFile; //user selected file
  int selectedIndex; //index of selectedPath

  vector<directory_entry> selectedFileChildren; //vector of children of selected files (if selected file is a directory)
  vector<directory_entry*> selectedFileChildrenFiltered; //vector of children of selected files (if selected file is a directory)
  string selectedFileContent; //content of selected file(if selected file is not a directory)

  void updateFiles(vector<directory_entry>&, const directory_entry&); //fills the vector with files from the specified directory
  void updateFiles(vector<directory_entry>&, const path&); //fills the vector with files from the specified path

  void updateSelectedData(); //updates selectedFilesChildren or selectedFileContent depending on selected file type
  
  bool applyNoneFilterCurrent(); //apply NONE filter for current files
  bool applyNoneFilterSelected(); //apply NONE filter for files in selected file if directory
  bool applyNoneFilter(); //apply NONE filter

public:
  FileManager(const directory_entry&); //constructor, initializes at given directory
  FileManager(); //constructor, initializes at current directory
  FileManager(const path&); //constructor, initializes at given path

  const path& getCurrentPath(); //gets the current directory program is in

  vector<directory_entry*>::const_iterator filesBegin(); //begin of const iterator to read files in current directory
  vector<directory_entry*>::const_iterator filesEnd(); //end of const iterator to read files in current directory
  vector<string> *getCurrentFilesString(); //gets pointer to vector of current file names
  int *getSelectedIndex(); //gets pointer to selectedIndex
  const directory_entry& getSelectedFile(); //gets selected file

  bool selectFile(directory_entry&, bool skipCheck=false); //selects the given file
  bool selectFile(path&, bool skipCheck=false); //selelcts the given file
  bool selectFile(); //updates the selected file based on index
  bool selectFile(int); //updates the index and selected file
  bool incrementSelected(bool cycle=true); //selects next file in the current directory
  bool decrementSelected(bool cycle=true); //selects previous file in the current directory
  bool selectParentDir(bool overRide=false); //selects the parent directory
  
  bool isSelectedDirectory(); //returns true if selected file is a directory

  vector<directory_entry*>::const_iterator selectedFilesBegin(); //begin of const iterator to read files in selected file
  vector<directory_entry*>::const_iterator selectedFilesEnd(); //end of const iterator to read files in selected file
  const string& getSelectedFileContent(); //returns content of selected file

  const path& switchPath(const directory_entry, bool skipCheck=false); //switchs to given directory
  const path& switchPath(path&, bool skipCheck=false); //switchs to given path
  const path& switchPath(); //switches to selected directory (selectedPath)
  const path& switchToParent(); //switches to parent directory
  
  bool applyFilter(FilterType); //applies filter to files in currentFiles and selectedFileChildren
  bool applyFilterCurrent(FilterType); //applies filter to files in currentFiles
  bool applyFilterSelected(FilterType); //applies filter to files in selectedFileChildren

  ~FileManager(); //destructor, does nothing as of now
};

#endif
