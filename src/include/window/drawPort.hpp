#pragma once

#include "../Engine3D/Engine3D.hpp"
#include "imgui.h"
#include "window.hpp"

#include "mainWindow.hpp"

class DrawPort : public Window {
public:
  DrawPort(const char *window_title) : Window(window_title) {};

  bool initialize() { return true; }

  void run() {
    const auto flags = ImGuiWindowFlags_NoCollapse |
                       ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove;

    auto &io = ImGui::GetIO();
    engine3D::Engine engine;

    ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;

    ImGui::SetNextWindowClass(&window_class);
    if (ImGui::Begin(this->window_title, NULL, flags)) {

      auto dt = 1.0f / (io.Framerate * 1.0f);
      MainWindow::m_theta += 1 * dt;

      engine.project(MainWindow::m_theta);

      ImGui::End();
    }
  }

  // Override for safety since for now this should never be possible
  void set_is_running(const bool new_is_running) {};
  void toggle_is_running() {};
};
