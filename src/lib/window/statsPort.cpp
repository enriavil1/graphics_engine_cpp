#include "../window/statsPort.hpp"
#include "../Engine3D/Engine3D.hpp"

#include "imgui.h"

bool StatsPort::initialize() {
  ImGui::MenuItem(this->window_title, NULL, &this->is_running);

  return true;
};

void StatsPort::run() {
  auto &io = ImGui::GetIO();

  engine3D::Object3D obj(engine3D::Mesh{}, 0, "");

  if (ImGui::Begin(this->window_title, &this->is_running,
                   ImGuiWindowFlags_AlwaysAutoResize)) {

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / io.Framerate, io.Framerate);

    if (engine3D::Engine::getProjectingObj(obj)) {
      ImGui::NewLine();

      const auto &obj_name = obj.getObjectName();
      const long &amount_of_triangles_projected =
          engine3D::Engine::getAmountOfTrianglesProjected();

      ImGui::Text("Object Name: %s", obj_name.c_str());
      ImGui::Text("Amount of triangles loaded: %ld",
                  obj.getMesh().triangles.size());
      ImGui::Text("Amount of vertices loaded: %d", obj.getAmountOfVertices());
      ImGui::NewLine();

      ImGui::Text("Amount of triangles being projected: %ld",
                  amount_of_triangles_projected);
      ImGui::Text("Amount of vertices being projected: %ld",
                  amount_of_triangles_projected * 3);

      ImGui::NewLine();
      ImGui::Checkbox("Toggle Object wire frame",
                      &engine3D::Engine::show_wire_frame);
    }

    ImGui::End();
  }
}
