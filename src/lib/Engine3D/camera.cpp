#include "../../include/Engine3D/camera.hpp"

#define STEP 1.0f

using namespace engine3D;

void Camera::moveUp(const double &theta) { this->p_position.y += STEP; }
void Camera::moveDown(const double &theta) { this->p_position.y -= STEP; }

void Camera::moveForward(const double &theta) {
  this->p_position = this->p_position + (this->p_direction * STEP);
}
void Camera::moveBackwards(const double &theta) {
  this->p_position = this->p_position - (this->p_direction * STEP);
}

void Camera::moveLeft(const double &theta) {
  const auto up_vector = Vec3D(0, 1, 0);
  auto right_vector = this->p_direction.getCrossProduct(up_vector).normalize();
  this->p_position = this->p_position - (right_vector * STEP);
}

void Camera::turnLeft(const double &theta) {
  this->p_y_rotation -= STEP * 0.5f;

  auto target = Vec3D(0, 0, 1);
  auto y_rotation_matrix = Matrix4x4::getYRotationMatrix(this->p_y_rotation);
  this->p_direction = target * y_rotation_matrix;
}

void Camera::moveRight(const double &theta) {
  const auto up_vector = Vec3D(0, 1, 0);
  auto right_vector = this->p_direction.getCrossProduct(up_vector).normalize();
  this->p_position = this->p_position + (right_vector * STEP);
}

void Camera::turnRight(const double &theta) {
  this->p_y_rotation += STEP * 0.5f;

  auto target = Vec3D(0, 0, 1);

  auto y_rotation_matrix = Matrix4x4::getYRotationMatrix(this->p_y_rotation);
  this->p_direction = target * y_rotation_matrix;
}

Vec3D Camera::getPos() { return this->p_position; }
Vec3D Camera::getDirection() { return this->p_direction; }

Matrix4x4 Camera::getLookAtMatrix() {
  auto forward = this->p_direction.normalize();

  auto up_vector = Vec3D(0, 1, 0);
  const auto a = forward * forward.getDotProduct(up_vector);
  up_vector = up_vector - a;
  up_vector = up_vector.normalize();

  const auto right_vector = up_vector.getCrossProduct(forward);

  Matrix4x4 point_at_matrix;
  point_at_matrix[0][0] = right_vector.x;
  point_at_matrix[0][1] = right_vector.y;
  point_at_matrix[0][2] = right_vector.z;
  point_at_matrix[1][0] = up_vector.x;
  point_at_matrix[1][1] = up_vector.y;
  point_at_matrix[1][2] = up_vector.z;
  point_at_matrix[2][0] = forward.x;
  point_at_matrix[2][1] = forward.y;
  point_at_matrix[2][2] = forward.z;
  point_at_matrix[3][0] = this->p_position.x;
  point_at_matrix[3][1] = this->p_position.y;
  point_at_matrix[3][2] = this->p_position.z;
  point_at_matrix[3][3] = 1.0f;

  return point_at_matrix.inverse();
}
