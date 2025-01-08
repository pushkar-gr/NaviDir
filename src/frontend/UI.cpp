#include "UI.hpp"
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/terminal.hpp>

//creates the root Component
//left: currentFiles
//right: selectedFiles
//bottom: cli
Component UI::createRootComp(Component& currentFiles, Component& selectedFiles, Component& cli) {
  return Renderer([=] {
    return vbox({
      hbox({
        currentFiles->Render() | size(WIDTH, EQUAL, Terminal::Size().dimx * .3),
        separator(),
        selectedFiles->Render(),
      }) | yflex,
      separator(),
      cli->Render(),
      afterRenderFunc()->Render(),
    });
  }) | CatchEvent([=] (Event event) {
    return handleInput(event);
  });
}

//creates component to display files in current directory
Component UI::createCurrentFilesComp(const vector<string> *data, int *index) { 
  MenuOption option = MenuOption::Vertical();
  option.entries_option.transform = [](EntryState state) {
    Element element = text(state.label);
    if (state.active) {
      element |= bgcolor(Color::White) | color(Color::Black);
    }
    return element;
  };
  return Menu(data, index, option) | vscroll_indicator | frame;
}

//creates component to display the content or files in selected file
Component UI::createSelectedFileComp(const string *data) { 
  return Renderer([=] {
      return paragraph(*data) | vscroll_indicator | frame;
  });
}

//creates cli component for user to input commands
Component UI::createCliComp(string *inputString) {
  return Renderer([=] {
    return text(*inputString);
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
  }
  switch (selectedElement) {
    case SelectedElement::currentFiles:
      if (event == Event::h || event == Event::ArrowRight) { //switch to parent
        fm->switchToParent();
      } else if (event == Event::l || event == Event::Return || event == Event::Tab || event == Event::ArrowLeft) { //switch to selected file
        if (fm->isSelectedDirectory()) {
          fm->switchPath();
        } else {
          selectedElement = SelectedElement::selectedFiles;
        }
      } else if (event == Event::j || event == Event::ArrowDown) { //select next file
        fm->incrementSelected();
      } else if (event == Event::k || event == Event::ArrowUp) { //seelct prev file
        fm->decrementSelected();
      }
      break;
    case SelectedElement::selectedFiles:
      if (event == Event::h || event == Event::ArrowLeft) { //selects back the file tree(currentFiles) 
        selectedElement = SelectedElement::currentFiles;
      } else if (event == Event::j || event == Event::ArrowDown) {
        //scroll down
      } else if (event == Event::k || event == Event::ArrowUp) {
        //scroll up
      }
      break;
    case SelectedElement::cli:
      if (event == Event::Return) {
        //process input and run
      } else if (event == Event::Backspace) { //backspace
        cliInput.pop_back();
      } else if (event == Event::Special(";")) { //append ':' instead of ';'
        cliInput.push_back(':');
      } else { //append user input
        cliInput += event.character();
      }
      break;
  }
  return false;
}

//constructor
//will get the data from backend and create UI
UI::UI(FileManager *fileManager) {
  fm = fileManager;

  auto screen = ScreenInteractive::Fullscreen();
  
  currentFiles = fm->getCurrentFilesString();
  currentIndex = fm->getSelectedIndex();
  currentFilesComp = createCurrentFilesComp(currentFiles, currentIndex);

  selectedFileData = fm->getSelectedDisplayContent();
  selectedFileComp = createSelectedFileComp(selectedFileData);

  cliComp = createCliComp(&cliInput);

  rootComp = createRootComp(currentFilesComp, selectedFileComp, cliComp);

  selectedElement = SelectedElement::currentFiles;
  
  screen.Loop(rootComp);
}

//destructor
//does nothing for now
UI::~UI() {}
