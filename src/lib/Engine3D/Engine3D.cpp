#include "../../../include/Engine3D/Engine3D.hpp"
#include "../../../include/Engine3D/matrix4x4/matrix4x4.hpp"

#include "imgui.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

using namespace engine3D;

Camera &Engine::getCamera() { return Engine::mp_camera; }

bool Engine::getProjectingObj(Object3D &obj) {
  auto projecting_obj = Engine::mp_projecting_obj.get();
  if (projecting_obj != nullptr) {
    obj = *projecting_obj;
    return true;
  }

  return false;
}

void Engine::project(double theta) {
  if (Engine::mp_projecting_obj == nullptr) {
    return;
  }

  auto camera_pos = Engine::mp_camera.getPos();
  ImDrawList *draw_list = ImGui::GetWindowDrawList();

  const float ASPECT_RATIO = ImGui::GetWindowHeight() / ImGui::GetWindowWidth();

  const auto &projection_matrix = Matrix4x4::getProjectionMatrix(ASPECT_RATIO);
  const auto &zx_rotation_matrix = Matrix4x4::getZXRotationMatrix(theta);

  std::vector<Triangle> triangles_to_draw;

  for (auto &tri : Engine::mp_projecting_obj->getMesh().triangles) {
    Triangle zx_rotated_triangle, projected_triangle;

    for (int i = 0; i < 3; ++i) {
      zx_rotated_triangle.points[i] = tri.points[i] * zx_rotation_matrix;
    }

    // offset the z axis
    for (Vec3D &point : zx_rotated_triangle.points) {
      point.z += (40.0f - camera_pos.z);
    }

    const auto normal = zx_rotated_triangle.getNormarl();
    const double dot_product =
        normal.getDotProduct(zx_rotated_triangle.points[0] - camera_pos);

    if (dot_product < 0.0) {
      projected_triangle.points[0] =
          zx_rotated_triangle.points[0] * projection_matrix;
      projected_triangle.points[1] =
          zx_rotated_triangle.points[1] * projection_matrix;
      projected_triangle.points[2] =
          zx_rotated_triangle.points[2] * projection_matrix;

      // scale projection point
      Engine::scaleTriangle(projected_triangle);
      triangles_to_draw.push_back(projected_triangle);
    }
  }

  std::sort(triangles_to_draw.begin(), triangles_to_draw.end(),
            [](Triangle &triangle_1, Triangle &triangle_2) {
              double triangle_1_avg_z = 0.0;
              for (auto &point : triangle_1.points) {
                triangle_1_avg_z += point.z;
              }

              double triangle_2_avg_z = 0.0;
              for (auto &point : triangle_2.points) {
                triangle_2_avg_z += point.z;
              }
              triangle_1_avg_z /= 3;

              return triangle_1_avg_z > triangle_2_avg_z;
            });

  const auto &window_pos = ImGui::GetWindowPos();

  std::array<ImVec2, 3> drawing_points;
  for (auto &projected_triangle : triangles_to_draw) {
    for (int i = 0; i < projected_triangle.points.size(); ++i) {
      drawing_points[i] = projected_triangle.points[i].getImVec2(window_pos);
    }

    draw_list->AddTriangleFilled(drawing_points[0], drawing_points[1],
                                 drawing_points[2], IM_COL32_WHITE);

    if (Engine::show_wire_frame) {
      draw_list->AddLine(drawing_points[0], drawing_points[1], IM_COL32_BLACK);
      draw_list->AddLine(drawing_points[0], drawing_points[2], IM_COL32_BLACK);
      draw_list->AddLine(drawing_points[1], drawing_points[2], IM_COL32_BLACK);
    }
  }
};

void Engine::scaleTriangle(Triangle &triangle) {
  for (Vec3D &point : triangle.points) {
    Engine::scaleVec2d(point);
  }
};

void Engine::scaleVec2d(ImVec2 &point) {
  point.x = (point.x + 1.0f) * ImGui::GetWindowWidth() * 0.5f;
  point.y = (point.y + 1.0f) * ImGui::GetWindowHeight() * 0.5f;
}

void Engine::scaleVec2d(Vec3D &point) {
  point.x = (point.x + 1.0f) * ImGui::GetWindowWidth() * 0.5f;
  point.y = (point.y + 1.0f) * ImGui::GetWindowHeight() * 0.5f;
}

bool Engine::loadObject(std::string file_path) {
  if (file_path.substr(file_path.find_last_of(".")) != ".obj") {
    return false;
  }

  std::ifstream file(file_path);
  if (!file.is_open()) {
    return false;
  }

  // local cache of vertices
  std::vector<Vec3D> vertices;

  Mesh mesh_loaded;

  std::string line;
  while (!file.eof()) {
    std::getline(file, line);
    std::stringstream stream_line(line);

    char character;

    switch (line[0]) {
    case 'v': {
      switch (line[1]) {
      case ' ': {
        Vec3D vertex;
        stream_line >> character >> vertex.x >> vertex.y >> vertex.z;
        vertices.push_back(vertex);
      } break;
      default:
        std::cout << "Missing symbol: " << line.substr(0, 2) << "\n";
      }
    } break;

    case 'f': {
      int vertices_index[4];
      int vertices_normal_index[4];
      int vertices_texture_index[4];
      char junk;

      stream_line >> character;

      const auto has_separator = line.find_first_of('/') <= line.size();

      for (int i = 0; i < 4 && stream_line.rdbuf()->in_avail() > 0; ++i) {
        if (has_separator) {
          stream_line >> vertices_index[i] >> junk >>
              vertices_texture_index[i] >> junk >> vertices_normal_index[i];
        } else {
          stream_line >> vertices_index[i];
        }
      }

      mesh_loaded.triangles.push_back(Triangle(
          {vertices[vertices_index[0] - 1], vertices[vertices_index[1] - 1],
           vertices[vertices_index[2] - 1]}));

      // if we have more means that the shape is made up of polygons so we
      // have to add an extra triangle for the polygon to be filled
      if (stream_line.rdbuf()->in_avail() > 0) {
        mesh_loaded.triangles.push_back(Triangle(
            {vertices[vertices_index[0] - 1], vertices[vertices_index[2] - 1],
             vertices[vertices_index[3] - 1]}));
      }

    } break;

    // skip comments and empty lines
    case ' ':
    case '#':
      break;

    default:
      std::cout << "Missing symbol: " << line[0] << "\n";
    }
  }

  if (mesh_loaded.triangles.size() == 0) {
    return false;
  }

  auto file_name = file_path.substr(file_path.find_last_of('/') + 1);

  auto obj = std::make_shared<Object3D>(mesh_loaded, vertices.size(),
                                        file_name.c_str());

  Engine::mp_loaded_objects.push_back(obj);
  Engine::mp_projecting_obj = obj;

  return true;
}
