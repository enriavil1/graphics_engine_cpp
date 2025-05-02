#include "../../include/Engine3D/camera.hpp"

#define STEP 32.0f
#define MOUSE_SPEED 0.005f

using namespace engine3D;

double Camera::getNear() { return this->p_near; }
double Camera::getFar() { return this->p_far; }

Vec3D Camera::getPos() { return this->p_position; }
Vec3D Camera::getDirection() { return this->p_direction; }

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
  const auto up_vector = Vec3D(0, 1, 0);

  auto right_vector = this->p_direction.getCrossProduct(up_vector).normalize();
  this->p_position = this->p_position - (right_vector * STEP * theta);
}

void Camera::turnLeft(const double &theta) {
  this->p_y_rotation -= STEP * 0.5f * theta;

  const auto &y_rotation_matrix =
      Matrix4x4::getYRotationMatrix(this->p_y_rotation);
  this->p_direction = this->p_target * y_rotation_matrix;
}

void Camera::moveRight(const double &theta) {
  const auto up_vector = Vec3D(0, 1, 0);

  auto right_vector = this->p_direction.getCrossProduct(up_vector).normalize();
  this->p_position = this->p_position + (right_vector * STEP * theta);
}

void Camera::turnRight(const double &theta) {
  this->p_y_rotation += STEP * 0.5f * theta;

  auto y_rotation_matrix = Matrix4x4::getYRotationMatrix(this->p_y_rotation);
  this->p_direction = this->p_target * y_rotation_matrix;
}

void Camera::turnUp(const double &theta) {
  this->p_x_rotation += STEP * 0.5f * theta;

  const auto &x_rotation_matrix =
      Matrix4x4::getXRotationMatrix(this->p_x_rotation);
  this->p_direction = this->p_target * x_rotation_matrix;
}

void Camera::turnDown(const double &theta) {
  this->p_x_rotation -= STEP * 0.5f * theta;

  auto x_rotation_matrix = Matrix4x4::getXRotationMatrix(this->p_x_rotation);
  this->p_direction = this->p_target * x_rotation_matrix;
}

void Camera::cameraTurn(const double &theta, const double &new_cursor_x_pos,
                        const double &new_cursor_y_pos) {
  const auto x_difference = new_cursor_x_pos - this->p_x_cursor_prev_pos;
  this->p_x_cursor_prev_pos = new_cursor_x_pos;
  const auto y_difference = new_cursor_y_pos - this->p_y_cursor_prev_pos;
  this->p_y_cursor_prev_pos = new_cursor_y_pos;

  this->p_x_rotation += MOUSE_SPEED * theta * x_difference;
  this->p_y_rotation += MOUSE_SPEED * theta * y_difference;

  const auto &x_rotation_matrix =
      Matrix4x4::getXRotationMatrix(this->p_x_rotation);
  const auto &y_rotation_matrix =
      Matrix4x4::getYRotationMatrix(this->p_y_rotation);

  this->p_direction = this->p_target * (x_rotation_matrix * y_rotation_matrix);
}

Matrix4x4 Camera::getLookAtMatrix() {
  auto forward = this->p_direction.normalize();

  auto right_vector = Vec3D(sinf(this->p_x_rotation - 3.14f / 2.0f), 0,
                            cosf(this->p_x_rotation - 3.14f / 2.0f));

  auto up_vector = forward.getCrossProduct(right_vector);

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
