#include "backend/FileManager.hpp"
#include "frontend/UI.hpp"
#include "config/Config.hpp"

int main() {
  Config config;
  FileManager fm(&config);
  UI ui(&fm, &config);
  return 0;
}
