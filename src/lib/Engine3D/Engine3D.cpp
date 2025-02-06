#include "../../../include/Engine3D/Engine3D.hpp"

#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace engine3D;

int Engine::getAmountOfTrianglesProjecting() {
  return Engine::p_projecting_mesh.triangles.size();
}

void Engine::project(double theta) {

  ImDrawList *draw_list = ImGui::GetWindowDrawList();

  const float NEAR = 0.1f;
  const float FAR = 1000.0f;
  const float FOV = 90.0f;
  const float ASPECT_RATIO = ImGui::GetWindowHeight() / ImGui::GetWindowWidth();

  const float FOV_RAD = 1.0f / tanf(FOV * 0.5f / 180.0f * 3.1415f);

  Matrix4x4 projection_matrix = {0};

  projection_matrix[0][0] = ASPECT_RATIO * FOV_RAD;
  projection_matrix[1][1] = FOV_RAD;
  projection_matrix[2][2] = FAR / (FAR - NEAR);
  projection_matrix[3][2] = (-FAR * NEAR) / (FAR - NEAR);
  projection_matrix[2][3] = 1.0f;
  projection_matrix[3][3] = 0.0f;

  // Matrix used for the rotation of the object
  Matrix4x4 z_rotation_matrix = {0};
  z_rotation_matrix[0][0] = cosf(theta);
  z_rotation_matrix[0][1] = sinf(theta);
  z_rotation_matrix[1][0] = -sinf(theta);
  z_rotation_matrix[1][1] = cosf(theta);
  z_rotation_matrix[2][2] = 1;
  z_rotation_matrix[3][3] = 1;

  Matrix4x4 x_rotation_matrix = {0};
  x_rotation_matrix[0][0] = 1;
  x_rotation_matrix[1][1] = cosf(theta * 0.5f);
  x_rotation_matrix[1][2] = sinf(theta * 0.5f);
  x_rotation_matrix[2][1] = -sinf(theta * 0.5f);
  x_rotation_matrix[2][2] = cosf(theta * 0.5f);
  x_rotation_matrix[3][3] = 1;

  for (auto tri : Engine::p_projecting_mesh.triangles) {
    Triangle z_rotated_triangle, zx_rotated_triangle, projected_triangle;

    Engine::multiplyVectorMatrix(tri.points[0], z_rotated_triangle.points[0],
                                 z_rotation_matrix);
    Engine::multiplyVectorMatrix(tri.points[1], z_rotated_triangle.points[1],
                                 z_rotation_matrix);
    Engine::multiplyVectorMatrix(tri.points[2], z_rotated_triangle.points[2],
                                 z_rotation_matrix);

    Engine::multiplyVectorMatrix(z_rotated_triangle.points[0],
                                 zx_rotated_triangle.points[0],
                                 x_rotation_matrix);
    Engine::multiplyVectorMatrix(z_rotated_triangle.points[1],
                                 zx_rotated_triangle.points[1],
                                 x_rotation_matrix);
    Engine::multiplyVectorMatrix(z_rotated_triangle.points[2],
                                 zx_rotated_triangle.points[2],
                                 x_rotation_matrix);
    // offset the z axis
    for (Vec3D &point : zx_rotated_triangle.points) {
      point.z += 40.0f;
    }

    const auto normal = zx_rotated_triangle.getNormarl();
    const double dot_product =
        normal.x * (zx_rotated_triangle.points[0].x - Engine::camera.x) +
        normal.y * (zx_rotated_triangle.points[0].y - Engine::camera.y) +
        normal.z * (zx_rotated_triangle.points[0].z - Engine::camera.z);

    if (dot_product < 0.0) {

      Engine::multiplyVectorMatrix(zx_rotated_triangle.points[0],
                                   projected_triangle.points[0],
                                   projection_matrix);
      Engine::multiplyVectorMatrix(zx_rotated_triangle.points[1],
                                   projected_triangle.points[1],
                                   projection_matrix);
      Engine::multiplyVectorMatrix(zx_rotated_triangle.points[2],
                                   projected_triangle.points[2],
                                   projection_matrix);

      // scale projection point
      Engine::scaleTriangle(projected_triangle);

      const auto &window_pos = ImGui::GetWindowPos();
      std::array<ImVec2, 3> drawing_points;
      for (int i = 0; i < projected_triangle.points.size(); ++i) {
        drawing_points[i] = projected_triangle.points[i].getImVec2(window_pos);
      }

      draw_list->AddTriangleFilled(drawing_points[0], drawing_points[1],
                                   drawing_points[2], IM_COL32_WHITE);
    }
  }
};

void Engine::multiplyVectorMatrix(const Vec3D &point, Vec3D &output,
                                  const Matrix4x4 &m) {

  output.x =
      point.x * m[0][0] + point.y * m[1][0] + point.z * m[2][0] + m[3][0];
  output.y =
      point.x * m[0][1] + point.y * m[1][1] + point.z * m[2][1] + m[3][1];
  output.z =
      point.x * m[0][2] + point.y * m[1][2] + point.z * m[2][2] + m[3][2];

  const float w =
      point.x * m[0][3] + point.y * m[1][3] + point.z * m[2][3] + m[3][3];

  if (w != 0.0f) {
    output.x /= w;
    output.y /= w;
    output.z /= w;
  }
};

void Engine::scaleTriangle(Triangle &triangle) {
  for (Vec3D &point : triangle.points) {
    point.x = (point.x + 1.0f) * ImGui::GetWindowWidth() * 0.5f;
    point.y = (point.y + 1.0f) * ImGui::GetWindowHeight() * 0.5f;
  }
};

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
      Vec3D vertex;
      stream_line >> character >> vertex.x >> vertex.y >> vertex.z;
      vertices.push_back(vertex);
    } break;

    case 'f': {
      int vertices_index[3];
      stream_line >> character >> vertices_index[0] >> vertices_index[1] >>
          vertices_index[2];

      auto triangle = Triangle({vertices[vertices_index[0] - 1],
                                vertices[vertices_index[1] - 1],
                                vertices[vertices_index[2] - 1]});
      mesh_loaded.triangles.push_back(triangle);
    } break;

    default:
      std::cout << "Missing symbol: " << line[0] << "\n";
    }
  }

  if (mesh_loaded.triangles.size() == 0) {
    return false;
  }

  auto file_name = file_path.substr(file_path.find_last_of('/') + 1);
  Engine::p_loaded_objects.push_back(
      std::make_shared<Object3D>(mesh_loaded, file_name.c_str()));

  Engine::p_projecting_mesh = mesh_loaded;

  return true;
}
