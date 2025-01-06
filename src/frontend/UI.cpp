#include "UI.hpp"

//creates the root Component
//left: currentFiles
//right: selectedFiles
//bottom: cli
Component UI::createRootComp(Component& currentFiles, Component& selectedFiles, Component& cli) {
  Component layout = Container::Vertical({
    fm->isSelectedDirectory() ? currentFiles : Container::Horizontal({
      currentFiles,
      selectedFiles,
    }),
    cli,
  });
  return Renderer(layout, [=] {
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
  });
}

//creates component to display files in current directory
Component UI::createCurrentFilesComp(const vector<string> *data, int *index) { 
  return Menu(data, index);
}

//creates component to display the content or files in selected file
Component UI::createSelectedFileComp(const string *data) { 
  return Scroller(Renderer([=] {
    return paragraph(*data);
  })) | xflex;
}

//creates cli component for user to input commands
Component UI::createCliComp(string *inputString) {
  return Input(inputString) | xflex;
}

//runs after the root completes rendering
Component UI::afterRenderFunc() {
  fm->selectFile();
  return Renderer([=] {
    return emptyElement();
  });
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
  
  screen.Loop(rootComp);
}

//destructor
//does nothing for now
UI::~UI() {}
