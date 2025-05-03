#include "../window/drawPort.hpp"

#include "../Engine3D/Engine3D.hpp"
#include "../window/fileDialog.hpp"
#include "../window/mainWindow.hpp"

#include "imgui.h"
#include <cstdio>
#include <iostream>

void DrawPort::processEvents() {
  glfwPollEvents();
  // hide cursor and capture
  glfwSetInputMode(MainWindow::getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  printf("here");

  const auto &io = ImGui::GetIO();

  // handle camera turning
  const auto &mouse_pos = ImGui::GetMousePos();
  const auto &window_pos = ImGui::GetWindowPos();
  engine3D::Engine::getCamera().cameraTurn(MainWindow::m_theta,
                                           mouse_pos.x + window_pos.x,
                                           mouse_pos.y + window_pos.y);

  this->p_has_captured_mouse = !ImGui::IsKeyPressed(ImGuiKey_Escape);

  // handle keyboard events
  glfwSetKeyCallback(
      MainWindow::getWindow(),
      [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto &camera = engine3D::Engine::getCamera();
        const auto &elapsed_time = MainWindow::m_theta;
        switch (key) {
        // Moving keys
        case GLFW_KEY_W:
          camera.moveForward(elapsed_time);
          break;
        case GLFW_KEY_S:
          camera.moveBackwards(elapsed_time);
          break;
        case GLFW_KEY_A:
          camera.moveLeft(elapsed_time);
          break;
        case GLFW_KEY_D:
          camera.moveRight(elapsed_time);
          break;

        // turning keys
        case GLFW_KEY_UP:
          camera.turnUp(elapsed_time);
          break;
        case GLFW_KEY_DOWN:
          camera.turnDown(elapsed_time);
          break;
        case GLFW_KEY_LEFT:
          camera.turnLeft(elapsed_time);
          break;
        case GLFW_KEY_RIGHT:
          camera.turnRight(elapsed_time);
          break;
        }
      });
}

void DrawPort::run() {
  const auto flags = ImGuiWindowFlags_NoCollapse;

  const auto &io = ImGui::GetIO();

  ImGuiWindowClass window_class;
  window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;

  ImGui::SetNextWindowClass(&window_class);
  if (ImGui::Begin(this->window_title, NULL, flags)) {

    this->p_has_captured_mouse = ImGui::IsWindowHovered() &&
                                 ImGui::IsMouseClicked(ImGuiMouseButton_Left);

    if (this->p_has_captured_mouse && ImGui::IsWindowHovered() &&
        ImGui::IsWindowFocused()) {
      this->processEvents();
    }

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
