#include "../window/drawPort.hpp"

#include "../Engine3D/Engine3D.hpp"
#include "../window/fileDialog.hpp"
#include "../window/mainWindow.hpp"

#include "GLFW/glfw3.h"
#include "imgui.h"
#include <cstdio>
#include <iostream>

void drawTriangles(const std::vector<engine3D::Triangle> &triangles_to_draw) {
  if (triangles_to_draw.size() == 0) {
    return;
  }

  ImDrawList *draw_list = ImGui::GetWindowDrawList();

  std::array<ImVec2, 3> drawing_points;
  for (auto &projected_triangle : triangles_to_draw) {
    for (int i = 0; i < projected_triangle.points.size(); ++i) {
      drawing_points[i] =
          projected_triangle.points[i].getImVec2(ImGui::GetWindowPos());
    }

    draw_list->AddTriangleFilledMultiColor(
        drawing_points[0], drawing_points[1], drawing_points[2],
        projected_triangle.colors[0], projected_triangle.colors[1],
        projected_triangle.colors[2]);

    if (engine3D::Engine::m_show_wire_frame) {
      draw_list->AddLine(drawing_points[0], drawing_points[1], IM_COL32_BLACK);
      draw_list->AddLine(drawing_points[0], drawing_points[2], IM_COL32_BLACK);
      draw_list->AddLine(drawing_points[1], drawing_points[2], IM_COL32_BLACK);
      for (const auto &point : drawing_points) {
        draw_list->AddCircleFilled(point, 5, IM_COL32_BLACK);
      }
    }
  }
}

void DrawPort::processEvents() {
  // hide cursor and capture
  auto &io = ImGui::GetIO();
  io.WantCaptureMouse = true;
  io.WantCaptureKeyboard = true;

  // handle camera turning
  const auto &mouse_pos = ImGui::GetMousePos();
  engine3D::Engine::getCamera().cameraTurn(MainWindow::m_theta, mouse_pos.x,
                                           mouse_pos.y);

  auto &camera = engine3D::Engine::getCamera();
  const auto &elapsed_time = MainWindow::m_theta;

  if (ImGui::IsKeyDown(ImGuiKey_Escape) || !ImGui::IsWindowFocused()) {
    glfwSetInputMode(MainWindow::getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    this->p_has_captured_mouse = false;
  }

  if (ImGui::IsKeyDown(ImGuiKey_W)) {
    camera.moveForward(elapsed_time);
  }

  if (ImGui::IsKeyDown(ImGuiKey_S)) {
    camera.moveBackwards(elapsed_time);
  }

  if (ImGui::IsKeyDown(ImGuiKey_A)) {
    camera.moveLeft(elapsed_time);
  }

  if (ImGui::IsKeyDown(ImGuiKey_D)) {
    camera.moveRight(elapsed_time);
  }

  if (ImGui::IsKeyDown(ImGuiKey_UpArrow)) {
    camera.turnUp(elapsed_time);
  }

  if (ImGui::IsKeyDown(ImGuiKey_DownArrow)) {
    camera.turnDown(elapsed_time);
  }

  if (ImGui::IsKeyDown(ImGuiKey_LeftArrow)) {
    camera.turnLeft(elapsed_time);
  }

  if (ImGui::IsKeyDown(ImGuiKey_RightArrow)) {
    camera.turnRight(elapsed_time);
  }
}

void DrawPort::run() {
  const auto flags = ImGuiWindowFlags_NoCollapse;

  auto &io = ImGui::GetIO();

  ImGuiWindowClass window_class;
  window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;

  ImGui::SetNextWindowClass(&window_class);

  if (ImGui::Begin(this->window_title, NULL, flags)) {

    if (!this->p_has_captured_mouse && ImGui::IsWindowHovered() &&
        ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
      glfwSetInputMode(MainWindow::getWindow(), GLFW_CURSOR,
                       GLFW_CURSOR_DISABLED);

      this->p_has_captured_mouse = true;

      const auto &mouse_pos = ImGui::GetMousePos();
      const auto &window_pos = ImGui::GetWindowPos();
      engine3D::Engine::getCamera().setMousePos(
          ImVec2(mouse_pos.x, mouse_pos.y));
    }

    if (this->p_has_captured_mouse) {
      this->processEvents();
    }

    auto dt = 1.0f / io.Framerate;
    MainWindow::m_theta = dt;

    std::vector<engine3D::Triangle> triangles_to_draw;
    engine3D::Engine::project(triangles_to_draw, MainWindow::m_theta);

    drawTriangles(triangles_to_draw);

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
