#pragma once

#include "GLFW/glfw3.h"
#include "imgui.h"

class MainWindow {
public:
  static bool initialize(const char *window_title);
  static void run();

  // Global theta of the whole application
  static double m_theta;

private:
  static GLFWwindow *m_window;

  const static ImVec4 m_clear_color;
  const static int m_width, m_height;

  static void glfw_sleep(int milliseconds);
  static void glfw_error_callback(int error, const char *description);

  static void process_frame();
  static void process_events();
};
