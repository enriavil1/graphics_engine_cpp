#include "../../include/Engine3D/camera.hpp"

#define STEP 8.0f

using namespace engine3D;

void Camera::moveUp(const double &theta) { this->p_position.y += STEP * theta; }
void Camera::moveDown(const double &theta) {
  this->p_position.y -= STEP * theta;
}

void Camera::moveForward(const double &theta) {
  this->p_position = this->p_position + (this->p_direction * STEP * theta);
}
void Camera::moveBackwards(const double &theta) {
  this->p_position = this->p_position - (this->p_direction * STEP * theta);
}

void Camera::moveLeft(const double &theta) {
  this->p_position.x -= STEP * theta;
}

void Camera::turnLeft(const double &theta) {
  this->p_y_rotation -= STEP * theta;

  auto target = Vec3D(0, 0, 1);

  auto y_rotation_matrix = Matrix4x4::getYRotationMatrix(this->p_y_rotation);
  this->p_direction = this->p_position + (target * y_rotation_matrix);
}

void Camera::moveRight(const double &theta) {
  this->p_position.x += STEP * theta;
}

void Camera::turnRight(const double &theta) {
  this->p_y_rotation += STEP * theta;

  auto target = Vec3D(0, 0, 1);

  auto y_rotation_matrix = Matrix4x4::getYRotationMatrix(this->p_y_rotation);
  this->p_direction = this->p_position + (target * y_rotation_matrix);
}

Vec3D Camera::getPos() { return this->p_position; }
Vec3D Camera::getDirection() { return this->p_direction; }

Matrix4x4 Camera::getLookAtMatrix() {
  auto forward = this->p_direction - this->p_position;
  forward = forward.normalize(forward);

  auto up_vector = Vec3D(0, 1, 0);
  const auto a = forward * forward.getDotProduct(up_vector);
  up_vector = up_vector - a;
  up_vector = up_vector.normalize(up_vector);

  const auto right_vector = up_vector.getCrossProduct(forward);

  Matrix4x4 look_at_matrix;
  look_at_matrix[0][0] = right_vector.x;
  look_at_matrix[0][1] = right_vector.y;
  look_at_matrix[0][2] = right_vector.z;
  look_at_matrix[1][0] = up_vector.x;
  look_at_matrix[1][1] = up_vector.y;
  look_at_matrix[1][2] = up_vector.z;
  look_at_matrix[2][0] = forward.x;
  look_at_matrix[2][1] = forward.y;
  look_at_matrix[2][2] = forward.z;
  look_at_matrix[3][0] = this->p_position.x;
  look_at_matrix[3][1] = this->p_position.y;
  look_at_matrix[3][2] = this->p_position.z;
  look_at_matrix[3][3] = 1.0f;

  return look_at_matrix.inverse();
}
