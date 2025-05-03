#pragma once

#include "window.hpp"

class StatsPort : public Window {
public:
  StatsPort(const char *window_title) : Window(window_title) {};
  bool initialize();

  void run();

  void processEvents() {};
};
