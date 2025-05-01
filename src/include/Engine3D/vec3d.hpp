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

  ImVec4 getImVec4() const {
    return ImVec4(this->x, this->y, this->z, this->w);
  }

  ImVec2 getImVec2(const ImVec2 &window_pos) const {
    return ImVec2(this->x + window_pos.x, this->y + window_pos.y);
  }

  ImVec2 getImVec2() const { return ImVec2(this->x, this->y); };

  Vec3D operator+(const Vec3D &vec3d) const;

  Vec3D operator-(const Vec3D &vec3d) const;

  Vec3D operator*(const Vec3D &vec3d) const;
  Vec3D operator*(const Matrix4x4 &matrix) const;
  Vec3D operator*(const float &num) const;

  Vec3D operator/(const Vec3D &vec3d) const;
  Vec3D operator/(const float &num) const;

  float getDotProduct(const Vec3D &vec3d) const;
  float getDotProduct() const;

  float getVectorLength() const;

  Vec3D getCrossProduct(const Vec3D &vec3d) const;
  Vec3D normalize() const;
};

} // namespace engine3D
