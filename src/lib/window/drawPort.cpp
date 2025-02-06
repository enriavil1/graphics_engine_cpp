#include "../window/drawPort.hpp"

#include "../Engine3D/Engine3D.hpp"
#include "../window/fileDialog.hpp"
#include "../window/mainWindow.hpp"

#include "imgui.h"
#include <iostream>

void DrawPort::run() {
  const auto flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;

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

  // if user is selecting a file we dont need to continue simulating
  while (FileDialog::m_selecting_files) {
    std::cout << FileDialog::openDialog() << std::endl;
    FileDialog::m_selecting_files = false;
  }
}
