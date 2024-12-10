#pragma once

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
  };

  ImVec2 getImVec2() { return ImVec2(this->x, this->y); };
};

struct Triangle {
  Point points[3];
};

struct Mesh {
  std::vector<Triangle> triangles;
};

} // namespace engine3D
