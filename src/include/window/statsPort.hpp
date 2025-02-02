#pragma once

#include "imgui.h"
#include "window.hpp"

class StatsPort : public Window {
public:
  StatsPort(const char *window_title) : Window(window_title) {};
  bool initialize() {
    ImGui::MenuItem(this->window_title, NULL, &this->is_running);

    return true;
  };

  void run() {
    auto &io = ImGui::GetIO();
    auto b = false;

    if (ImGui::Begin(this->window_title, &this->is_running,
                     ImGuiWindowFlags_AlwaysAutoResize)) {

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                  1000.0f / io.Framerate, io.Framerate);

      ImGui::End();
    }
  }
};
