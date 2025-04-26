#include "../../include/Engine3D/matrix4x4/matrix4x4.hpp"

using namespace engine3D;

Row Matrix4x4::operator[](const int &index) const { return this->m[index]; }
Row &Matrix4x4::operator[](const int &index) { return this->m[index]; }

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &matrix) const {
  Matrix4x4 result;

  for (int r = 0; r < 4; ++r) {
    for (int c = 0; c < 4; ++c) {
      result[r][c] =
          this->m[r][0] * matrix[0][c] + this->m[r][1] * matrix[1][c] +
          this->m[r][2] * matrix[2][c] + this->m[r][3] * matrix[3][c];
    }
  }

  return result;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &matrix) {
  Matrix4x4 result;

  for (int r = 0; r < 4; ++r) {
    for (int c = 0; c < 4; ++c) {
      result[r][c] =
          this->m[r][0] * matrix[0][c] + this->m[r][1] * matrix[1][c] +
          this->m[r][2] * matrix[2][c] + this->m[r][3] * matrix[3][c];
    }
  }

  return result;
}

Matrix4x4 Matrix4x4::getWorldMatrix() {
  auto world_matrix = Matrix4x4();
  world_matrix[0][0] = 1.0f;
  world_matrix[1][1] = 1.0f;
  world_matrix[2][2] = 1.0f;
  world_matrix[3][3] = 1.0f;

  return world_matrix;
}

const Matrix4x4 Matrix4x4::getProjectionMatrix(const float &aspect_ratio) {
  const float NEAR = 0.1f;
  const float FAR = 1000.0f;
  const float FOV = 90.0f;
  const float FOV_RAD = 1.0f / tanf(FOV * 0.5f / 180.0f * 3.1415f);

  Matrix4x4 projection_matrix;
  projection_matrix[0][0] = aspect_ratio * FOV_RAD;
  projection_matrix[1][1] = FOV_RAD;
  projection_matrix[2][2] = FAR / (FAR - NEAR);
  projection_matrix[3][2] = (-FAR * NEAR) / (FAR - NEAR);
  projection_matrix[2][3] = 1.0f;
  projection_matrix[3][3] = 0.0f;

  return projection_matrix;
}

const Matrix4x4 Matrix4x4::getYRotationMatrix(const float &theta) {
  Matrix4x4 y_rotation_matrix;
  y_rotation_matrix[0][0] = cosf(theta);
  y_rotation_matrix[0][2] = sinf(theta);
  y_rotation_matrix[1][1] = 1;
  y_rotation_matrix[2][0] = -sinf(theta);
  y_rotation_matrix[2][2] = cosf(theta);
  y_rotation_matrix[3][3] = 1;

  return y_rotation_matrix;
}

const Matrix4x4 Matrix4x4::getZRotationMatrix(const float &theta) {
  Matrix4x4 z_rotation_matrix;
  z_rotation_matrix[0][0] = cosf(theta);
  z_rotation_matrix[0][1] = sinf(theta);
  z_rotation_matrix[1][0] = -sinf(theta);
  z_rotation_matrix[1][1] = cosf(theta);
  z_rotation_matrix[2][2] = 1;
  z_rotation_matrix[3][3] = 1;

  return z_rotation_matrix;
}

const Matrix4x4 Matrix4x4::getXRotationMatrix(const float &theta) {

  Matrix4x4 x_rotation_matrix;
  x_rotation_matrix[0][0] = 1;
  x_rotation_matrix[1][1] = cosf(theta * 0.5f);
  x_rotation_matrix[1][2] = sinf(theta * 0.5f);
  x_rotation_matrix[2][1] = -sinf(theta * 0.5f);
  x_rotation_matrix[2][2] = cosf(theta * 0.5f);
  x_rotation_matrix[3][3] = 1;

  return x_rotation_matrix;
}

const Matrix4x4 Matrix4x4::getZXRotationMatrix(const float &theta) {
  return Matrix4x4::getZRotationMatrix(theta) *
         Matrix4x4::getXRotationMatrix(theta);
}

// Inverse only works with translation and rotation matrices
Matrix4x4 Matrix4x4::inverse() {

  Matrix4x4 inversed_matrix;
  inversed_matrix[0][0] = this->m[0][0];
  inversed_matrix[0][1] = this->m[1][0];
  inversed_matrix[0][2] = this->m[2][0];
  inversed_matrix[0][3] = 0.0f;

  inversed_matrix[1][0] = this->m[0][1];
  inversed_matrix[1][1] = this->m[1][1];
  inversed_matrix[1][2] = this->m[2][1];
  inversed_matrix[1][3] = 0.0f;

  inversed_matrix[2][0] = this->m[0][2];
  inversed_matrix[2][1] = this->m[1][2];
  inversed_matrix[2][2] = this->m[2][2];
  inversed_matrix[2][3] = 0.0f;

  inversed_matrix[3][0] = -(this->m[3][0] * inversed_matrix[0][0] +
                            this->m[3][1] * inversed_matrix[1][0] +
                            this->m[3][2] * inversed_matrix[2][0]);
  inversed_matrix[3][1] = -(this->m[3][0] * inversed_matrix[0][1] +
                            this->m[3][1] * inversed_matrix[1][1] +
                            this->m[3][2] * inversed_matrix[2][1]);
  inversed_matrix[3][2] = -(this->m[3][0] * inversed_matrix[0][2] +
                            this->m[3][1] * inversed_matrix[1][2] +
                            this->m[3][2] * inversed_matrix[2][2]);
  inversed_matrix[3][3] = 1.0f;

  return inversed_matrix;
}
