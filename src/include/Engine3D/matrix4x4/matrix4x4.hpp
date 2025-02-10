#pragma once

#include "../vec3d.hpp"
#include "row.hpp"

namespace engine3D {

class Matrix4x4 {
private:
  Row m[4];

public:
  Matrix4x4() {};
  Matrix4x4(float input_matrix[4][4]) {
    for (int i = 0; i < 4; ++i) {
      m[i] = Row(input_matrix[i]);
    }
  };

  Row operator[](const int &index) const;
  Row &operator[](const int &index);

  Matrix4x4 operator*(const Matrix4x4 &matrix) const;
  Matrix4x4 operator*(const Matrix4x4 &matrix);

  Matrix4x4 operator*(const Vec3D &vec3d) const;
  Matrix4x4 operator*(const Vec3D &vec3d);

  static const Matrix4x4 getProjectionMatrix(const float &aspect_ratio);
  static const Matrix4x4 getZRotationMatrix(const float &theta);
  static const Matrix4x4 getXRotationMatrix(const float &theta);
  static const Matrix4x4 getZXRotationMatrix(const float &theta);
};
}; // namespace engine3D
