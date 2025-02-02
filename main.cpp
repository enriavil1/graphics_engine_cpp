#include "window/mainWindow.hpp"
#include <iostream>

int main() {
  const char *window_title = "3D Engine";
  if (!MainWindow::initialize(window_title)) {
    std::cerr << "Failed to initialize main window" << std::endl;
    return -1;
  }
  MainWindow::run();

  return 0;
}
