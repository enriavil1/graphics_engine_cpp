#include "../window/statsPort.hpp"
#include "../Engine3D/Engine3D.hpp"

#include "imgui.h"
#include <vector>

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

    std::vector<std::shared_ptr<engine3D::Object3D>> objects;
    if (engine3D::Engine::getProjectingObjects(objects)) {
      long amount_of_vertices = 0, amount_of_triangles_loaded = 0;
      for (const auto &obj : objects) {
        amount_of_triangles_loaded += obj->getMesh().triangles.size();
        amount_of_vertices += obj->getAmountOfVertices();
      }

      ImGui::NewLine();

      const long &amount_of_triangles_projected =
          engine3D::Engine::getAmountOfTrianglesProjected();

      ImGui::Text("Amount of triangles loaded: %ld",
                  amount_of_triangles_loaded);
      ImGui::Text("Amount of vertices loaded: %ld", amount_of_vertices);
      ImGui::NewLine();

      ImGui::Text("Amount of triangles being projected: %ld",
                  amount_of_triangles_projected);
      ImGui::Text("Amount of vertices being projected: %ld",
                  amount_of_triangles_projected * 3);

      ImGui::NewLine();
      ImGui::Checkbox("Toggle object wire frame",
                      &engine3D::Engine::m_show_wire_frame);
      ImGui::Checkbox("Toggle triangle clipping debug",
                      &engine3D::Engine::m_show_triangle_clipping);
    }

    ImGui::End();
  }
}
