#include "../../include/Engine3D/camera.hpp"

#define STEP 4.0f

using namespace engine3D;

void Camera::moveUp() { this->p_position.y += 2.0f; }
void Camera::moveDown() { this->p_position.y -= 2.0f; }

void Camera::moveForward() {
  this->p_position = this->p_position + this->p_direction * 4.0f;
}
void Camera::moveBackwards() {
  this->p_position = this->p_position - this->p_direction * 4.0f;
}

void Camera::moveLeft() { this->p_position.x -= 1.0f; }

void Camera::turnLeft() {
  this->p_y_rotation -= 2.0f;
  auto y_rotation_matrix = Matrix4x4::getYRotationMatrix(this->p_y_rotation);
  this->p_direction =
      this->p_position + (this->p_direction * y_rotation_matrix);
}

void Camera::moveRight() { this->p_position.x += 1.0f; }

void Camera::turnRight() {
  this->p_y_rotation += 2.0f;
  auto y_rotation_matrix = Matrix4x4::getYRotationMatrix(this->p_y_rotation);
  this->p_direction =
      this->p_position + (this->p_direction * y_rotation_matrix);
}

Vec3D Camera::getPos() { return this->p_position; }
Vec3D Camera::getDirection() { return this->p_direction; }

Matrix4x4 Camera::getLookAtMatrix() {
  auto forward = this->p_direction - this->p_position;
  forward = forward.normalize(forward);

  const auto a = forward * forward.getDotProduct(this->up_vector);
  this->up_vector = this->up_vector - a;
  this->up_vector = this->up_vector.normalize(this->up_vector);

  const auto right_vector = this->up_vector.getCrossProduct(forward);

  Matrix4x4 look_at_matrix;
  look_at_matrix[0][0] = right_vector.x;
  look_at_matrix[0][1] = right_vector.y;
  look_at_matrix[0][2] = right_vector.z;
  look_at_matrix[1][0] = this->up_vector.x;
  look_at_matrix[1][1] = this->up_vector.y;
  look_at_matrix[1][2] = this->up_vector.z;
  look_at_matrix[2][0] = forward.x;
  look_at_matrix[2][1] = forward.y;
  look_at_matrix[2][2] = forward.z;
  look_at_matrix[3][0] = this->p_position.x;
  look_at_matrix[3][1] = this->p_position.y;
  look_at_matrix[3][2] = this->p_position.z;
  look_at_matrix[3][3] = 1.0f;

  return look_at_matrix.inverse();
}
