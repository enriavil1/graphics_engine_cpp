#pragma once

#include <imgui.h>
#include <vector>

namespace engine3D {

struct Triangle {
  ImVec4 points[3];
};

struct Mesh {
  std::vector<Triangle> triangles;
};

struct Point {
  double x;
  double y;
  double z;

  ImVec4 getImVec4() {
    return ImVec4(this->x, this->y, this->z,
                  0 /* We dont need the last coordinate */);
  }
};

} // namespace engine3D
