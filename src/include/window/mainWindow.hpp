#pragma once

#include "GLFW/glfw3.h"
#include "imgui.h"

class MainWindow {
public:
  static bool initialize();
  static void run();

  static GLFWwindow *getWindow();

  // Global theta of the whole application
  static double m_theta;

private:
  static GLFWwindow *mp_window;

  const static ImVec4 mp_clear_color;
  const static int mp_width, mp_height;

  static void glfw_sleep(int milliseconds);
  static void glfw_error_callback(int error, const char *description);

  static void process_frame();
  static void process_events();
};
