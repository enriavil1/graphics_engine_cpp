#pragma once

#include <array>
#include <imgui.h>
#include <vector>

namespace engine3D {

struct Point {
  double x;
  double y;
  double z;

  ImVec4 getImVec4() {
    return ImVec4(this->x, this->y, this->z,
                  0 /* We dont need the last coordinate */);
  }

  ImVec2 getImVec2(const ImVec2 &window_pos) {
    return ImVec2(this->x + window_pos.x, this->y + window_pos.y);
  }

  ImVec2 getImVec2() { return ImVec2(this->x, this->y); };
};

struct Triangle {
  std::array<Point, 3> points;
  ImU32 color = IM_COL32_WHITE;

  Point getNormarl() {
    // A line is a length between two points
    Point lineA = {.x = points[1].x - points[0].x,
                   .y = points[1].y - points[0].y,
                   .z = points[1].z - points[0].z};

    Point lineB = {.x = points[2].x - points[0].x,
                   .y = points[2].y - points[0].y,
                   .z = points[2].z - points[0].z};

    Point normal = {.x = lineA.y * lineB.z - lineA.z * lineB.y,
                    .y = lineA.z * lineB.x - lineA.x * lineB.z,
                    .z = lineA.x * lineB.y - lineA.y * lineB.x};

    // normalize the normal
    float normalizer =
        sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

    normal.x /= normalizer;
    normal.y /= normalizer;
    normal.z /= normalizer;

    return normal;
  };
};

struct Mesh {
  std::vector<Triangle> triangles;
};

} // namespace engine3D
