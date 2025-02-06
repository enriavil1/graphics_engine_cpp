#include "../window/statsPort.hpp"

#include "imgui.h"

bool StatsPort::initialize() {
  ImGui::MenuItem(this->window_title, NULL, &this->is_running);

  return true;
};

void StatsPort::run() {
  auto &io = ImGui::GetIO();

  if (ImGui::Begin(this->window_title, &this->is_running,
                   ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / io.Framerate, io.Framerate);

    ImGui::End();
  }
}
