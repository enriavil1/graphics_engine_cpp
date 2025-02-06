#include "window/mainWindow.hpp"
#include <iostream>

int main() {
  if (!MainWindow::initialize()) {
    std::cerr << "Failed to initialize main window" << std::endl;
    return -1;
  }
  MainWindow::run();

  return 0;
}
