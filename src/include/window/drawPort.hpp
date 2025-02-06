#pragma once

#include "window.hpp"

class DrawPort : public Window {
public:
  DrawPort(const char *window_title) : Window(window_title) {};

  bool initialize() { return true; }

  void run();

  // Override for safety since for now this should never be possible
  void set_is_running(const bool new_is_running) {};
  void toggle_is_running() {};
};
