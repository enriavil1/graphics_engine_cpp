#pragma once

#include <imgui.h>

namespace engine3D {

class Point {
public:
  double x = 0;
  double y = 0;
  double z = 0;

  Point() {};
  Point(double x, double y, double z) : x(x), y(y), z(z) {};

  ImVec4 getImVec4() {
    return ImVec4(this->x, this->y, this->z,
                  0 /* We dont need the last coordinate */);
  }

  ImVec2 getImVec2(const ImVec2 &window_pos) {
    return ImVec2(this->x + window_pos.x, this->y + window_pos.y);
  }

  ImVec2 getImVec2() { return ImVec2(this->x, this->y); };
};

} // namespace engine3D
