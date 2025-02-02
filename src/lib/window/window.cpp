#include "../../../src/include/window/window.hpp"

void Window::set_window_width(int width) { this->width = width; }
void Window::set_window_height(int height) { this->height = height; }
void Window::set_window_clear_color(const ImVec4 &clear_color) {
  Window::clear_color = clear_color;
}

bool Window::get_is_running() { return this->is_running; }
void Window::toggle_is_running() { this->is_running = !this->is_running; }
void Window::set_is_running(const bool new_is_running) {
  this->is_running = new_is_running;
}
