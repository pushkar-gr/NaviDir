#include "UI.hpp"
#include <ftxui/component/component_base.hpp>

//creates the root Component
//left: currentFiles
//right: selectedFiles
//bottom: cli
Component UI::createRootComp(Component& currentDir, Component& currentFiles, Component& selectedFiles, Component& cli, Component& cliOutput) {
  return Renderer([=] {
    return vbox({
      currentDir->Render() | xflex,
      separator(),
      hbox({
        currentFiles->Render() | size(WIDTH, EQUAL, Terminal::Size().dimx * .3),
        separator(),
        selectedFiles->Render() | size(WIDTH, EQUAL, Terminal::Size().dimx * .7),
      }) | yflex,
      separator(),
      cli->Render(),
      separator() | color(Color::GrayDark),
      cliOutput->Render(),
      afterRenderFunc()->Render(),
    });
  }) | CatchEvent([=] (Event event) {
    return handleInput(event);
  });
}

//creates component to display directory program is in
Component UI::createCurrentDirComp(const path *currentDir) {
  return Renderer([=] {
    return text((*currentDir).string()) | center;
  }) | xflex;
}

//creates component to display files in current directory
Component UI::createCurrentFilesComp(const vector<string> *data, int *index) { 
  MenuOption option = MenuOption::Vertical();
  option.entries_option.transform = [this](EntryState state) {
    Element element = text(state.label);
    if (state.active) {
      if (selectedElement == SelectedElement::currentFiles) {
        element |= bgcolor(Color::White) | color(Color::Black);
      } else {
        element |= bgcolor(Color::GrayDark) | color(Color::White);
      }
    }
    return element;
  };
  return Menu(data, index, option) | vscroll_indicator | frame;
}

//creates component to display the content or files in selected file
Component UI::createSelectedFileComp(const string *data) { 
  return Renderer([=] {
    return paragraph(*data) | vscroll_indicator | hscroll_indicator  | focusPositionRelative(focus_x, focus_y) | frame;
  });
}

//creates cli component for user to input commands
Component UI::createCliComp(string *inputString) {
  return Renderer([=] {
    return text(*inputString);
  }) | xflex;
}

//creates component to display cli output
Component UI::createCliOutputComp(string *outputString) {
  return Renderer([=] {
    return text(*outputString);
  }) | xflex;
}

//runs after the root completes rendering
Component UI::afterRenderFunc() {
  return Renderer([=] {
    return emptyElement();
  });
}

//handles user input
bool UI::handleInput(Event event) {
  if ((event == Event::Special(":") || event == Event::Special(";")) && selectedElement != SelectedElement::cli) { //selects cli if user input is ; or :
    selectedElement = SelectedElement::cli;
    if (cliInput.size() != 0) {
      return false;
    }
  }
  switch (selectedElement) {
    case SelectedElement::currentFiles:
      if (event == Event::h || event == Event::ArrowRight) { //switch to parent
        fm->switchToParent();
        focus_x = focus_y = 0.f;
      } else if (event == Event::l || event == Event::Return || event == Event::Tab || event == Event::ArrowLeft) { //switch to selected file
        if (fm->isSelectedDirectory()) {
          fm->switchPath();
          focus_x = focus_y = 0.f;
        } else {
          selectedElement = SelectedElement::selectedFiles;
        }
      } else if (event == Event::j || event == Event::ArrowDown) { //select next file
        fm->incrementSelected();
        focus_x = focus_y = 0.f;
      } else if (event == Event::k || event == Event::ArrowUp) { //seelct prev file
        fm->decrementSelected();
        focus_x = focus_y = 0.f;
      }
      break;
    case SelectedElement::selectedFiles:
      if (event == Event::j || event == Event::ArrowDown) { //scroll down
        focus_y += .1;
        focus_y = focus_y > 1 ? 1 : focus_y;
      } else if (event == Event::k || event == Event::ArrowUp) { //scroll up
        focus_y -= .1;
        focus_y = focus_y < 0 ? 0 : focus_y;
      } else if (event == Event::CtrlH || event == Event::Backspace) { //scroll left
        focus_x -= .1;
        focus_x = focus_x < 0 ? 0 : focus_x;
      } else if (event == Event::CtrlL) { //scroll right
        focus_x += .1;
        focus_x = focus_x > 1 ? 1 : focus_x;
      } else if (event == Event::h || event == Event::ArrowLeft) { //selects back the file tree(currentFiles) 
        selectedElement = SelectedElement::currentFiles;
      }
      break;
    case SelectedElement::cli:
      if (event == Event::Return) {
        //process input and run
        selectedElement = SelectedElement::currentFiles;
      } else if (event == Event::Backspace) { //backspace
        cliInput.pop_back();
      } else if (event == Event::Special(";")) { //append ':' instead of ';'
        cliInput.push_back(':');
      } else if (event == Event::Escape) {
        selectedElement = SelectedElement::currentFiles;
      } else { //append user input
        cliInput += event.character();
      }
      break;
  }
  return false;
}

//displays output in cliOutputComp
void UI::displayOutput(string output) {
  cliOutput = output;
}

//constructor
//will get the data from backend and create UI
UI::UI(FileManager *fileManager) {
  fm = fileManager;

  auto screen = ScreenInteractive::Fullscreen();
  
  currentDirectory = fm->getCurrentPath();
  currentDirectoryComp = createCurrentDirComp(currentDirectory);

  currentFiles = fm->getCurrentFilesString();
  currentIndex = fm->getSelectedIndex();
  currentFilesComp = createCurrentFilesComp(currentFiles, currentIndex);

  focus_x = 0.f;
  focus_y = 0.f;
  selectedFileData = fm->getSelectedDisplayContent();
  selectedFileComp = createSelectedFileComp(selectedFileData);

  cliComp = createCliComp(&cliInput);

  cliOutputComp = createCliOutputComp(&cliOutput);

  rootComp = createRootComp(currentDirectoryComp, currentFilesComp, selectedFileComp, cliComp, cliOutputComp);

  selectedElement = SelectedElement::currentFiles;

  displayOutput("Opened directory " + (*currentDirectory).string());
  
  screen.Loop(rootComp);
}

//destructor
//does nothing for now
UI::~UI() {}
