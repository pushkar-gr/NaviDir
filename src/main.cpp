#include "backend/FileManager.hpp"
#include "frontend/UI.hpp"
#include "config/Config.hpp"
#include <filesystem>

int main(int size, char **args) {
  Config config;
  FileManager *fm;
  if (size == 2) {
    path path = current_path()/args[1];
    if (exists(path)) {
      fm = new FileManager(path, &config);
    } else {
      fm = new FileManager(&config);
    }
  } else {
    fm = new FileManager(&config);
  }
  UI ui(fm, &config);
  return 0;
}
