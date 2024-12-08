#include "../../../include/Engine3D/Engine3D.hpp"
#include "imgui.h"
#include <cmath>

using namespace engine3D;

void Engine::project(double theta) {

  ImDrawList *draw_list = ImGui::GetWindowDrawList();

  const float NEAR = 0.1f;
  const float FAR = 1000.0f;
  const float FOV = 90.0f;
  const float ASPECT_RATIO = ImGui::GetWindowHeight() / ImGui::GetWindowWidth();

  const float FOV_RAD = 1.0f / tanf(FOV * 0.5f / 180.0f * 3.1415f);

  Matrix4x4 projection_matrix;

  projection_matrix.m[0][0] = ASPECT_RATIO * FOV_RAD;
  projection_matrix.m[1][1] = FOV_RAD;
  projection_matrix.m[2][2] = FAR / (FAR - NEAR);
  projection_matrix.m[3][2] = (-FAR * NEAR) / (FAR - NEAR);
  projection_matrix.m[2][3] = 1.0f;
  projection_matrix.m[3][3] = 0.0f;

  auto window_pos = ImGui::GetWindowPos();

  // Matrix used for the rotation of the object
  Matrix4x4 z_rotation_matrix, x_rotation_matrix;
  z_rotation_matrix.m[0][0] = cosf(theta);
  z_rotation_matrix.m[0][1] = sinf(theta);
  z_rotation_matrix.m[1][0] = -sinf(theta);
  z_rotation_matrix.m[1][1] = cosf(theta);
  z_rotation_matrix.m[2][2] = 1;
  z_rotation_matrix.m[3][3] = 1;

  x_rotation_matrix.m[0][0] = 1;
  x_rotation_matrix.m[1][1] = cosf(theta * 0.5f);
  x_rotation_matrix.m[1][2] = sinf(theta * 0.5f);
  x_rotation_matrix.m[2][1] = -sinf(theta * 0.5f);
  x_rotation_matrix.m[2][2] = cosf(theta * 0.5f);
  x_rotation_matrix.m[3][3] = 1;

  for (auto tri : this->p_mesh_cube.triangles) {
    Triangle z_rotated_triangle, x_rotated_triangle, projected_triangle;

    this->multiplyVectorMatrix(tri.points[0], z_rotated_triangle.points[0],
                               z_rotation_matrix);
    this->multiplyVectorMatrix(tri.points[1], z_rotated_triangle.points[1],
                               z_rotation_matrix);
    this->multiplyVectorMatrix(tri.points[2], z_rotated_triangle.points[2],
                               z_rotation_matrix);

    this->multiplyVectorMatrix(z_rotated_triangle.points[0],
                               x_rotated_triangle.points[0], x_rotation_matrix);
    this->multiplyVectorMatrix(z_rotated_triangle.points[1],
                               x_rotated_triangle.points[1], x_rotation_matrix);
    this->multiplyVectorMatrix(z_rotated_triangle.points[2],
                               x_rotated_triangle.points[2], x_rotation_matrix);

    // offset the z axis
    x_rotated_triangle.points[0].z += 3.0f;
    x_rotated_triangle.points[1].z += 3.0f;
    x_rotated_triangle.points[2].z += 3.0f;

    this->multiplyVectorMatrix(x_rotated_triangle.points[0],
                               projected_triangle.points[0], projection_matrix);
    this->multiplyVectorMatrix(x_rotated_triangle.points[1],
                               projected_triangle.points[1], projection_matrix);
    this->multiplyVectorMatrix(x_rotated_triangle.points[2],
                               projected_triangle.points[2], projection_matrix);

    // scale projection point
    auto scale_point = [](ImVec4 &point) {
      point.x = (point.x + 1.0f) * ImGui::GetWindowWidth() * 0.5f;
      point.y = (point.y + 1.0f) * ImGui::GetWindowHeight() * 0.5f;
    };

    scale_point(projected_triangle.points[0]);
    scale_point(projected_triangle.points[1]);
    scale_point(projected_triangle.points[2]);

    // Scale into view

    ImVec2 point1 =
        ImVec2(projected_triangle.points[0].x + ImGui::GetWindowPos().x,
               projected_triangle.points[0].y + ImGui::GetWindowPos().y);
    ImVec2 point2 =
        ImVec2(projected_triangle.points[1].x + ImGui::GetWindowPos().x,
               projected_triangle.points[1].y + ImGui::GetWindowPos().y);
    ImVec2 point3 =
        ImVec2(projected_triangle.points[2].x + ImGui::GetWindowPos().x,
               projected_triangle.points[2].y + ImGui::GetWindowPos().y);

    draw_list->AddLine(point1, point2, IM_COL32_WHITE);
    draw_list->AddLine(point2, point3, IM_COL32_WHITE);
    draw_list->AddLine(point3, point1, IM_COL32_WHITE);
  }
};

void Engine::multiplyVectorMatrix(const ImVec4 &point, ImVec4 &output,
                                  const Matrix4x4 &m) {

  output.x = point.x * m.m[0][0] + point.y * m.m[1][0] + point.z * m.m[2][0] +
             m.m[3][0];
  output.y = point.x * m.m[0][1] + point.y * m.m[1][1] + point.z * m.m[2][1] +
             m.m[3][1];
  output.z = point.x * m.m[0][2] + point.y * m.m[1][2] + point.z * m.m[2][2] +
             m.m[3][2];

  const float w = point.x * m.m[0][3] + point.y * m.m[1][3] +
                  point.z * m.m[2][3] + m.m[3][3];

  if (w != 0.0f) {
    output.x /= w;
    output.y /= w;
    output.z /= w;
  }
};
