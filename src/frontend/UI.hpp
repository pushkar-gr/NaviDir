#ifndef UI_HPP
#define UI_HPP
#include "../backend/FileManager.hpp"
#include "../config/Config.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include <filesystem>
#include <ftxui/component/event.hpp>
#include <string>
#include <vector>

using namespace std;
using namespace ftxui;


enum SelectedElement {
  files, //file tree where all the files in current directory is displayed
  cli, //cli for user actions
};

enum UserInput {
  left,
  right,
  up,
  down,
  scrollLeft,
  scrollRight,
  scrollUp,
  scrollDown,
  createFile,
  renameFileName,
  renameFileWithExt,
  moveFile,
  deleteFile,
  copyFile,
  cutFile,
  pasteFile,
  find,
  cleanFind,
  toggleHiddenFiles,
  togglePermissions,
  toggleSize,
  toggleDateModified,
  refresh,
  quit,
};

class UI {
private:
  Component createRootComp(Component&, Component&, Component&, Component&); //creates the root component of the ui
  Component createCurrentDirComp(const path *); //creates a component to display current directory
  Component createCurrentFilesComp(const vector<string> *, int *); //creates the file component which displays all the files in current directory
  Component createSelectedFileComp(const string *); //creates the display component which displays the content or files in selected file
  Component createCliComp(string *, string *, string *); //creates the cli Component in which user can input cmds
  Component afterRenderFunc(); //a function that runs after rendering root component

  bool handleInput(Event); //handls input
  bool processInput(UserInput); //process user input
  bool processCliInput(); //process cli input

  SelectedElement selectedElement; //focused element
  UserInput inputAction; //user input

  FileManager *fm; //pointer to backend file manager
  Config *config; //pointer to config
  directory_entry copyCutFile; //user copied or cut file
  bool isCopyFile; //true => copy file, false => cut file

  ScreenInteractive screen; //TerminalScreen
  Component rootComp; //the root component

  const path *currentDirectory; //current directory program is in
  Component currentDirectoryComp; //component to display current directory

  const vector<string> *currentFiles; //names of files in current directory
  int *currentIndex; //index of selected file
  Component currentFilesComp; //component to display all the files in current directory
  
  float focus_x; //used to scroll selectedFileComp horizontally
  float focus_y; //used to scroll selectedFileComp vertically
  const string *selectedFileData; //content to display in selectedFileComp
  Component selectedFileComp; //component to display contents of selected file

  string cliText; //cli instruction
  string cliInput; //input from user
  string cliOutput; //cli output
  Component cliComp; //cli component

public:
  UI(FileManager *, Config *); //constructor to create UI

  ~UI(); //destructor, does nothing for now
};

#endif
