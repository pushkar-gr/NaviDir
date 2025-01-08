#ifndef UI_HPP
#define UI_HPP
#include "../backend/FileManager.hpp"
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "../scroller/scroller.hpp"
#include <ftxui/component/event.hpp>
#include <string>
#include <vector>

using namespace std;
using namespace ftxui;


enum SelectedElement {
  currentFiles, //file tree where all the files in current directory is displayed
  selectedFiles, //display content of selected file
  cli, //cli for user actions
};

class UI {
private:
  Component createRootComp(Component&, Component&, Component&); //creates the root component of the ui

  Component createCurrentFilesComp(const vector<string> *, int *); //creates the file component which displays all the files in current directory

  Component createSelectedFileComp(const string *); //creates the display component which displays the content or files in selected file

  Component createCliComp(string *); //creates the cli Component in which user can input cmds

  Component afterRenderFunc(); //a function that runs after rendering root component
  
  SelectedElement selectedElement;
  bool handleInput(Event);

  FileManager *fm; //pointer to backend file manager

  Component rootComp; //the root component

  const vector<string> *currentFiles; //names of files in current directory
  int *currentIndex; //index of selected file
  Component currentFilesComp; //component to display all the files in current directory
  
  const string *selectedFileData; //content to display in selectedFileComp
  Component selectedFileComp; //component to display contents of selected file

  string cliInput; //input for the cli
  Component cliComp; //cli component

public:
  UI(FileManager *fm); //constructor to create UI

  ~UI(); //destructor, does nothing for now
};

#endif
