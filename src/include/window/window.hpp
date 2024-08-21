#pragma once

#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>

#include <imgui.h>
#include <memory>
#include <unistd.h>

class Window {
public:
  explicit Window(int width, int height, ImVec4 clear_color);
  Window();
  ~Window();

  static bool initialize(const char *window_title);
  static void run();

  static void set_window_height(int height);
  static void set_window_width(int width);
  static void set_window_clear_color(const ImVec4 &clear_color);

  static ImGuiIO &get_window_io();

private:
  static bool m_is_running;

  static ImVec4 m_clear_color;

  static std::unique_ptr<ImGuiIO> m_io;

  static GLFWwindow *m_window;
  static int m_width;
  static int m_height;

  static void glfw_sleep(int milliseconds);
  static void glfw_error_callback(int error, const char *description);

  static void process_frame();
  static void process_events();
};

inline Window::Window() {
  Window::m_is_running = false;

  Window::m_clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  Window::m_io = nullptr;

  Window::m_width = 1280;
  Window::m_height = 720;
}

inline Window::Window(int width, int height, ImVec4 clear_color) {
  Window::m_is_running = false;

  Window::m_clear_color = clear_color;

  Window::m_io = nullptr;

  Window::m_width = width;
  Window::m_height = height;
}

inline Window::~Window() {}
