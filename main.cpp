#include "window/window.hpp"
#include <iostream>

int main() {
  const char *window_title = "3D Engine";
  if (!Window::initialize(window_title)) {
    std::cerr << "Failed to initialize main window" << std::endl;
    return -1;
  }
  Window::run();

  return 0;
}
