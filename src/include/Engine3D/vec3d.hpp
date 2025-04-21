#pragma once

#include "matrix4x4/matrix4x4.hpp"
#include <imgui.h>

namespace engine3D {

class Vec3D {
public:
  double x = 0;
  double y = 0;
  double z = 0;
  double w = 1;

  Vec3D() {};
  Vec3D(double x, double y, double z) : x(x), y(y), z(z) {};

  ImVec4 getImVec4() { return ImVec4(this->x, this->y, this->z, this->w); }

  ImVec2 getImVec2(const ImVec2 &window_pos) {
    return ImVec2(this->x + window_pos.x, this->y + window_pos.y);
  }

  ImVec2 getImVec2() { return ImVec2(this->x, this->y); };

  Vec3D operator+(const Vec3D &vec3d);

  Vec3D operator-(const Vec3D &vec3d);

  Vec3D operator*(const Vec3D &vec3d);
  Vec3D operator*(const Matrix4x4 &matrix);
  Vec3D operator*(const float &num);

  Vec3D operator/(const Vec3D &vec3d);

  float getDotProduct(const Vec3D &vec3d) const;
  float getVectorLength(const Vec3D &vec3d);

  Vec3D getCrossProduct(const Vec3D &vec3d);
  Vec3D normalize(const Vec3D &vec3d);
};

} // namespace engine3D
