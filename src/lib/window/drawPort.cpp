#include "../window/drawPort.hpp"

#include "../Engine3D/Engine3D.hpp"
#include "../window/fileDialog.hpp"
#include "../window/mainWindow.hpp"

#include "imgui.h"
#include <iostream>

void DrawPort::run() {
  const auto flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar;

  auto &io = ImGui::GetIO();

  ImGuiWindowClass window_class;
  window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;

  ImGui::SetNextWindowClass(&window_class);
  if (ImGui::Begin(this->window_title, NULL, flags)) {

    auto dt = 1.0f / io.Framerate;
    MainWindow::m_theta = dt;

    engine3D::Engine::project(MainWindow::m_theta);

    ImGui::End();
  }

  std::string selected_file_path = "";

  // if user is selecting a file we dont need to continue simulating
  while (FileDialog::m_selecting_files) {
    selected_file_path = FileDialog::openDialog();
    FileDialog::m_selecting_files = false;
  }

  if (selected_file_path != "") {
    if (!engine3D::Engine::loadObject(selected_file_path)) {
      std::cout << "Mesh was not loaded\n";
    }
  }
}
