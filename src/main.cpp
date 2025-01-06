#include "backend/FileManager.hpp"
#include "frontend/UI.hpp"

int main() {
  FileManager fm;
  UI ui(&fm);
  return 0;
}
