#include "../../../include/Engine3D/Engine3D.hpp"
#include "../../../include/Engine3D/matrix4x4/matrix4x4.hpp"

#include "imgui.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace engine3D;

Camera &Engine::getCamera() { return Engine::mp_camera; }

long Engine::getAmountOfTrianglesProjected() {
  return Engine::mp_amount_of_triangles_projected;
}

bool Engine::getProjectingObjects(
    std::vector<std::shared_ptr<Object3D>> &objects) {
  objects = Engine::mp_loaded_objects;
  return objects.size() > 0;
}

void Engine::project(std::vector<Triangle> &triangles_to_project,
                     double theta) {

  const auto &window_pos = ImGui::GetWindowPos();
  auto camera_pos = Engine::getCamera().getPos();

  const float ASPECT_RATIO = ImGui::GetWindowHeight() / ImGui::GetWindowWidth();

  auto world_matrix = Matrix4x4::getWorldMatrix() *
                      Matrix4x4::getTranslationMatrix(0.0f, 0.0f, 5.0f);

  const auto &projection_matrix = Matrix4x4::getProjectionMatrix(
      Engine::getCamera().getNear(), ASPECT_RATIO);
  const auto &view_matrix = Engine::getCamera().getLookAtMatrix();

  // as we load the obj we have the vertecis inverted
  const auto &xy_inversion_vector = Vec3D(-1.0f, -1.0f, 1.0f);

  // restart for stats
  Engine::mp_amount_of_triangles_projected = 0;

  for (const auto &object : Engine::mp_loaded_objects) {

    std::vector<Triangle> triangles_to_draw;
    std::deque<Triangle> clipped_triangles;

    for (const auto &tri : object->getMesh().triangles) {
      Triangle projected_triangle;

      for (int i = 0; i < projected_triangle.points.size(); ++i) {
        projected_triangle.points[i] = tri.points[i] * world_matrix;
      }

      const auto normal = projected_triangle.getNormarl();

      if (normal.getDotProduct(projected_triangle.points[0] - camera_pos) <
          0.0) {

        for (int i = 0; i < projected_triangle.points.size(); ++i) {
          // TODO: Add proper light entity
          // for now the camera view = light
          auto light_direction = Engine::getCamera().getDirection().normalize();

          // get the point based on the camera
          auto point = projected_triangle.points[i] - camera_pos;

          auto distance = 1.0f / std::fmax(point.getDotProduct(),
                                           Engine::getCamera().getNear());

          auto light_level =
              (light_direction.getDotProduct(point.normalize()) * distance) * 5;
          light_level = std::fmin(1.0f, std::fmax(light_level, 0.0f));

          const auto color = IM_COL32(255 * light_level, 255 * light_level,
                                      255 * light_level, 255);
          projected_triangle.colors[i] = color;

          projected_triangle.points[i] =
              projected_triangle.points[i] * view_matrix;
        }

        // clip after view matrix multiplaction for z value
        Engine::clipTriangle(Vec3D(0.0f, 0.0f, Engine::getCamera().getNear()),
                             Vec3D(0.0f, 0.0f, Engine::getCamera().getFar()),
                             projected_triangle, clipped_triangles);

        // we might have clipped triangles to project as well
        for (auto &triangle : clipped_triangles) {

          for (int i = 0; i < 3; ++i) {
            triangle.points[i] = triangle.points[i] * projection_matrix;

            triangle.points[i] = (triangle.points[i] / triangle.points[i].w) *
                                 xy_inversion_vector;
          }
          // scale projection point
          Engine::scaleTriangle(triangle);
          triangles_to_draw.push_back(triangle);
        }
        clipped_triangles = {}; // flush clipped triangles since this in this
                                // step should have a size of 1 or 2
      }
    }

    // pairs of points to a plane in the unit vector
    std::array<std::pair<Vec3D, Vec3D>, 4> planes = {
        // bottom most plane
        std::make_pair(Vec3D(0, 0, 0), Vec3D(0, 1, 0)),
        // top most plane
        std::make_pair(Vec3D(0, ImGui::GetWindowHeight() - 1, 0),
                       Vec3D(0, -1, 0)),

        // left most plane
        std::make_pair(Vec3D(0, 0, 0), Vec3D(1, 0, 0)),
        // right most plane
        std::make_pair(Vec3D(ImGui::GetWindowWidth() - 1, 0, 0),
                       Vec3D(-1, 0, 0)),
    };

    std::deque<Triangle> clipped_triangles_to_draw;
    for (const auto &triangle : triangles_to_draw) {
      clipped_triangles_to_draw.push_back(triangle);
    }
    for (const auto &plane : planes) {
      auto current_amount_to_clip = clipped_triangles_to_draw.size();
      while (current_amount_to_clip > 0) {
        current_amount_to_clip += Engine::clipTriangle(
            plane.first, plane.second, clipped_triangles_to_draw.front(),
            clipped_triangles_to_draw);
        clipped_triangles_to_draw.pop_front();
        --current_amount_to_clip;
      }
    }

    Engine::mp_amount_of_triangles_projected +=
        clipped_triangles_to_draw.size();

    for (const auto &triangle : clipped_triangles_to_draw) {
      triangles_to_project.push_back(triangle);
    }
  }

  // sort the triangles to draw from back to front
  std::sort(triangles_to_project.begin(), triangles_to_project.end(),
            [](const Triangle &triangle_1, const Triangle &triangle_2) {
              double triangle_1_avg_z = 0.0;
              for (const auto &point : triangle_1.points) {
                triangle_1_avg_z += point.z;
              }

              double triangle_2_avg_z = 0.0;
              for (const auto &point : triangle_2.points) {
                triangle_2_avg_z += point.z;
              }

              return (triangle_1_avg_z / 3) > (triangle_2_avg_z / 3);
            });
};

Vec3D Engine::getPlaneInterception(
    const Vec3D &point_on_plane, const Vec3D &plane, const Vec3D &start_of_line,
    const Vec3D &end_of_line) { // for safety normalize the plane
  const auto &plane_normal = plane.normalize();
  const auto &point_on_plane_normal =
      plane_normal.getDotProduct(point_on_plane);

  // getting plane interception is just:
  // plane_x * point_x + plane_y * point_y + plane_z * point_z
  const auto &start_of_line_interception =
      plane_normal.getDotProduct(start_of_line);
  const auto &end_of_line_interception =
      plane_normal.getDotProduct(end_of_line);

  /*
   *
   * Because a point = { x, y, z}
   * point.x = mt + b
   * point.y = mt + b
   * point.z = mt + b
   *
   * we have to input those points into our plane normal and solve for t
   * the line * t = the line to point of interception
   *
   * */

  float t = (point_on_plane_normal - start_of_line_interception) /
            (end_of_line_interception - start_of_line_interception);

  const auto &line_from_start_to_end = end_of_line - start_of_line;
  const auto &line_to_intersect = line_from_start_to_end * t;

  // return point of interception
  // essentially mx+b
  return line_to_intersect + start_of_line;
}

int Engine::clipTriangle(const Vec3D &point_on_plane, const Vec3D &plane,
                         const Triangle &triangle,
                         std::deque<Triangle> &triangles_output) {

  const auto &plane_normal = plane.normalize();
  std::pair<Vec3D, ImU32> inside_points[2];
  int inside_point_idx = 0;

  std::pair<Vec3D, ImU32> outside_points[2];
  int outside_point_idx = 0;

  for (int i = 0; i < 3; ++i) {
    const auto &point = triangle.points[i];
    const auto &color = triangle.colors[i];

    const float &dst_from_point_to_plane =
        plane_normal.getDotProduct(point) -
        plane_normal.getDotProduct(point_on_plane);

    if (dst_from_point_to_plane >= -0.1) {
      // if all points are inside we just return the current triangle

      if (inside_point_idx == 2) {
        triangles_output.push_back(triangle);
        return 0;
      }

      inside_points[inside_point_idx] = std::make_pair(point, color);
      ++inside_point_idx;
    } else {
      // if all points are outside we just return
      // and dont add this point back

      if (outside_point_idx == 2) {
        return 0;
      }
      outside_points[outside_point_idx] = std::make_pair(point, color);
      ++outside_point_idx;
    }
  }

  // we have two points inside and one outside
  if (inside_point_idx == 2) {
    Triangle new_triangle_1, new_triangle_2;

    // we fill in the valid points
    new_triangle_1.points[0] = inside_points[0].first;

    new_triangle_2.points[0] = inside_points[1].first;
    new_triangle_1.points[1] = inside_points[1].first;

    const auto &new_point_1 = Engine::getPlaneInterception(
        point_on_plane, plane_normal, inside_points[0].first,
        outside_points[0].first);

    new_triangle_1.points[2] = new_point_1;
    new_triangle_2.points[1] = new_point_1;

    new_triangle_2.points[2] = Engine::getPlaneInterception(
        point_on_plane, plane_normal, inside_points[1].first,
        outside_points[0].first);

    new_triangle_1.colors[0] = inside_points[0].second;
    new_triangle_1.colors[1] = inside_points[1].second;
    new_triangle_1.colors[2] = outside_points[0].second;

    new_triangle_2.colors[0] = inside_points[1].second;
    new_triangle_2.colors[1] = outside_points[0].second;
    new_triangle_2.colors[2] = outside_points[0].second;

    if (Engine::m_show_triangle_clipping) {
      const auto green = IM_COL32(0, 255, 0, 255);
      const auto red = IM_COL32(255, 0, 0, 255);

      new_triangle_1.colors = {green, green, green};
      new_triangle_2.colors = {red, red, red};
    }

    triangles_output.push_back(new_triangle_1);
    triangles_output.push_back(new_triangle_2);
    return 2;
  }

  // we have two points outside the plane
  Triangle new_triangle;

  // we fill in the valid points
  new_triangle.points[0] = inside_points[0].first;

  // we get the interception to the plane and set them as points
  new_triangle.points[1] = Engine::getPlaneInterception(
      point_on_plane, plane_normal, inside_points[0].first,
      outside_points[0].first);
  new_triangle.points[2] = Engine::getPlaneInterception(
      point_on_plane, plane_normal, inside_points[0].first,
      outside_points[1].first);

  new_triangle.colors[0] = inside_points[0].second;
  new_triangle.colors[1] = outside_points[0].second;
  new_triangle.colors[2] = outside_points[1].second;

  if (Engine::m_show_triangle_clipping) {
    const auto blue = IM_COL32(0, 0, 255, 255);

    new_triangle.colors = {blue, blue, blue};
  }
  triangles_output.push_back(new_triangle);
  return 1;
};

void Engine::scaleTriangle(Triangle &triangle) {
  for (Vec3D &point : triangle.points) {
    Engine::scaleVec2d(point);
  }
};

void Engine::scaleVec2d(ImVec2 &point) {
  point.x = (point.x + 1.0f) * (ImGui::GetWindowWidth() * 0.5f);
  point.y = (point.y + 1.0f) * (ImGui::GetWindowHeight() * 0.5f);
}

void Engine::scaleVec2d(Vec3D &point) {
  point.x = (point.x + 1.0f) * (ImGui::GetWindowWidth() * 0.5f);
  point.y = (point.y + 1.0f) * (ImGui::GetWindowHeight() * 0.5f);
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

        vertex = vertex + Engine::getCamera().getPos() +
                 Engine::getCamera().getDirection();

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

  return true;
}
