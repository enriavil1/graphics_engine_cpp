#pragma once

#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>

#include <imgui.h>

class Window {
public:
  explicit Window(const char *window_title, int width, int height,
                  ImVec4 clear_color);
  Window(const char *window_title);
  ~Window();

  virtual bool initialize() = 0;
  virtual void run() = 0;
  virtual void processEvents() = 0;

  void set_window_height(int height);
  void set_window_width(int width);
  void set_window_clear_color(const ImVec4 &clear_color);

  bool get_is_running();
  void set_is_running(const bool new_is_running);
  void toggle_is_running();

protected:
  bool is_running;
  const char *window_title;

  ImVec4 clear_color;

  int width;
  int height;
};

inline Window::Window(const char *window_title) : window_title(window_title) {
  Window::is_running = true;

  Window::clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  Window::width = 1280;
  Window::height = 720;
}

inline Window::Window(const char *window_title, int width, int height,
                      ImVec4 clear_color)
    : window_title(window_title) {
  Window::is_running = true;

  Window::clear_color = clear_color;

  Window::width = width;
  Window::height = height;
}

inline Window::~Window() {}
