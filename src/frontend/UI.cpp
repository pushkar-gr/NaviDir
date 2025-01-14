#include "UI.hpp"
#include "TextFormat.hpp"
#include <filesystem>
#include <ftxui/component/screen_interactive.hpp>

//creates the root Component
//left: currentFiles
//right: selectedFiles
//bottom: cli
Component UI::createRootComp(Component& currentDir, Component& currentFiles, Component& selectedFiles, Component& cli) {
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
      afterRenderFunc()->Render(),
    });
  }) | CatchEvent([=] (Event event) {
    return handleInput(event);
  });
}

//creates component to display directory program is in
Component UI::createCurrentDirComp(const path *currentDir) {
  return Renderer([=] {
    return text(formatText(currentDir->string(), FormatType::Simple)) | center;
  }) | xflex;
}

//creates component to display files in current directory
Component UI::createCurrentFilesComp(const vector<string> *data, int *index) { 
  MenuOption option = MenuOption::Vertical();
  option.entries_option.transform = [=](EntryState state) {
    Element element = text(state.label);
    if (state.active) {
      if (selectedElement == SelectedElement::files) {
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
Component UI::createCliComp(string *cliText, string *cliInput) {
  return Renderer([=] {
    return text(*cliText + *cliInput);
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
  if (selectedElement == SelectedElement::cli) {
    if (event == Event::Return) {
      //process cli input
      selectedElement = SelectedElement::files;
    } else if (event == Event::Escape) {
      cliText = cliInput = "";
      selectedElement = SelectedElement::files;
    } else if (event == Event::Backspace) {
      if (cliInput.size() > 0) {
        cliInput.pop_back();
      }
    } else {
      cliInput += event.character();
    }
  } else if (event == Event::h || event == Event::ArrowLeft) {
    return processInput(UserInput::left);
  } else if (event == Event::l || event == Event::ArrowRight || (event == Event::Return && fm->isSelectedDirectory())) {
    return processInput(UserInput::right);
  } else if (event == Event::k || event == Event::ArrowUp) {
    return processInput(UserInput::up);
  } else if (event == Event::j || event == Event::ArrowDown) {
    return processInput(UserInput::down);
  } else if (event == Event::CtrlH || event == Event::ArrowLeftCtrl || event == Event::Backspace || (event.is_mouse() && event.mouse().button == Mouse::WheelLeft)) {
    return processInput(UserInput::scrollLeft);
  } else if (event == Event::CtrlL || event == Event::ArrowRightCtrl || (event.is_mouse() && event.mouse().button == Mouse::WheelRight)) {
    return processInput(UserInput::scrollRight);
  } else if (event == Event::CtrlK || event == Event::ArrowUpCtrl || (event.is_mouse() && event.mouse().button == Mouse::WheelUp)) {
    return processInput(UserInput::scrollUp);
  } else if (event == Event::CtrlJ || event == Event::ArrowDownCtrl || (event == Event::Return && ! fm->isSelectedDirectory()) || (event.is_mouse() && event.mouse().button == Mouse::WheelDown)) {
    return processInput(UserInput::scrollDown);
  } else if (event == Event::a || event == Event::CtrlN) {
    return processInput(UserInput::createFile);
  } else if (event == Event::e) {
    return processInput(UserInput::renameFileName);
  } else if (event == Event::r) {
    return processInput(UserInput::renameFileWithExt);
  } else if (event == Event::u) {
    return processInput(UserInput::moveFile);
  } else if (event == Event::d) {
    return processInput(UserInput::deleteFile);
  } else if (event == Event::y || event == Event::CtrlC) {
    return processInput(UserInput::copyFile);
  } else if (event == Event::Y || event == Event::CtrlX) {
    return processInput(UserInput::cutFile);
  } else if (event == Event::p || event == Event::CtrlP) {
    return processInput(UserInput::pasteFile);
  } else if (event == Event::Special("/") || event == Event::f || event == Event::CtrlF) {
    return processInput(UserInput::find);
  } else if (event == Event::F) {
    return processInput(UserInput::cleanFind);
  } else if (event == Event::v) {
    return processInput(UserInput::toggleHiddenFiles);
  } else if (event == Event::b) {
    return processInput(UserInput::togglePermissions);
  } else if (event == Event::n) {
    return processInput(UserInput::toggleSize);
  } else if (event == Event::m) {
    return processInput(UserInput::toggleDateModified);
  } else if (event == Event::q) {
    return processInput(UserInput::quit);
  }
  return false;
}

bool UI::processInput(UserInput input) {
  inputAction = input;
  switch (input) {
    case UserInput::left: {
      fm->switchToParent();
      focus_x = focus_y = 0;
      break;
    }

    case UserInput::right: {
      fm->switchPath();
      focus_x = focus_y = 0;
      break;
    }

    case UserInput::up: {
      fm->decrementSelected();
      focus_x = focus_y = 0;
      break;
    }

    case UserInput::down: {
      fm->incrementSelected();
      focus_x = focus_y = 0;
      break;
    }

    case UserInput::scrollLeft: {
      focus_x -= .1;
      focus_x = focus_x < 0 ? 0 : focus_x;
      break;
    }

    case UserInput::scrollRight: {
      focus_x += .1;
      focus_x = focus_x > 1 ? 1 : focus_x;
      break;
    }

    case UserInput::scrollUp: {
      focus_y -= .1;
      focus_y = focus_y < 0 ? 0 : focus_y;
      break;
    }

    case UserInput::scrollDown: {
      focus_y += .1;
      focus_y = focus_y > 1 ? 1 : focus_y;
      break;
    }

    case UserInput::createFile: {
      cliText = "Create file ";
      cliInput = currentDirectory->string() + "/";
      selectedElement = SelectedElement::cli;
      break;
    }

    case UserInput::renameFileName: {
      cliInput = fm->getSelectedFile().path().filename();
      size_t pos = cliInput.find_last_of('.');
      if (pos != string::npos && pos != 0) {
        cliInput.erase(pos);
      }
      cliText = "Rename file " + cliInput + " to: ";
      selectedElement = SelectedElement::cli;
      break;
    }

    case UserInput::renameFileWithExt: {
      cliInput = fm->getSelectedFile().path().filename();
      cliText = "Rename file " + cliInput + " to: ";
      selectedElement = SelectedElement::cli;
      break;
    }

    case UserInput::moveFile: {
      cliText = "Move file to ";
      cliInput = formatText(*currentDirectory, FormatType::Simple);
      selectedElement = SelectedElement::cli;
      break;
    }

    case UserInput::deleteFile: {
      cliText = "Remove " + formatText(fm->getSelectedFile().path().filename(), FormatType::Simple) + "? y/N: ";
      cliInput = "";
      selectedElement = SelectedElement::cli;
      break;
    }

    case UserInput::copyFile: {
      copyCutFile = fm->getSelectedFile();
      cliText = "Copied file " + formatText(copyCutFile.path().filename(), FormatType::Simple);
      break;
    }

    case UserInput::cutFile: {
      copyCutFile = fm->getSelectedFile();
      cliText = "Cut file " + formatText(copyCutFile.path().filename(), FormatType::Simple);
      break;
    }

    case UserInput::pasteFile: {
      if (! copyCutFile.exists()) {
        cliText = "Copy or cut a file to paste";
        return false;
      }
      //paste file code
      cliText = "Pasted file " + formatText(copyCutFile, FormatType::Simple);
      break;
    }

    case UserInput::find: {
      cliText = "Filter: ";
      cliInput = "";
      break;
    }

    case UserInput::cleanFind: {
      cliText = "Cleared filter";
      config->setFilter("");
      break;
    }

    case UserInput::toggleHiddenFiles: {
      config->toggleHiddenFiles();
      break;
    }

    case UserInput::togglePermissions: {
      config->togglePerms();
      fm->refresh();
      break;
    }

    case UserInput::toggleSize: {
      config->toggleFileSize();
      fm->refresh();
      break;
    }

    case UserInput::toggleDateModified: {
      config->toggleDateMod();
      fm->refresh();
      break;
    }

    case UserInput::quit: { 
      screen.Exit();
      break;
    }
      
  }
  return true;
}

//constructor
//will get the data from backend and create UI
UI::UI(FileManager *fileManager, Config *inputConfig) : screen(ScreenInteractive::Fullscreen()) {
  fm = fileManager;

  config = inputConfig;

  currentDirectory = fm->getCurrentPath();
  currentDirectoryComp = createCurrentDirComp(currentDirectory);

  currentFiles = fm->getCurrentFilesString();
  currentIndex = fm->getSelectedIndex();
  currentFilesComp = createCurrentFilesComp(currentFiles, currentIndex);

  focus_x = 0.f;
  focus_y = 0.f;
  selectedFileData = fm->getSelectedDisplayContent();
  selectedFileComp = createSelectedFileComp(selectedFileData);

  cliComp = createCliComp(&cliText, &cliInput);

  rootComp = createRootComp(currentDirectoryComp, currentFilesComp, selectedFileComp, cliComp);

  selectedElement = SelectedElement::files;

  cliText = "Opened directory " + formatText(currentDirectory->string(), FormatType::Simple);
  
  screen.Loop(rootComp);
}

//destructor
//does nothing for now
UI::~UI() {}
