#include "../../include/Engine3D/camera.hpp"

#define STEP 8.0f
#define MOUSE_SPEED 0.25f

using namespace engine3D;

// Multiplies target by the x_matrix and y_matrix in a specific order to get
// direction use this as safety to avoid messing up the order
void multiplyForNewDirection(Vec3D &direction, const Vec3D &target,
                             const Matrix4x4 &x_matrix,
                             const Matrix4x4 &y_matrix) {
  direction = target * (x_matrix * y_matrix);
}

const double Camera::getNear() const { return this->p_near; }
const double Camera::getFar() const { return this->p_far; }

const Vec3D Camera::getPos() const { return this->p_position; }
const Vec3D Camera::getDirection() const { return this->p_direction; }

void Camera::setMousePos(ImVec2 mouse_pos) {
  this->p_x_cursor_prev_pos = mouse_pos.x;
  this->p_y_cursor_prev_pos = mouse_pos.y;
}

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
  this->p_yaw -= STEP * 0.5f * theta;

  const auto &x_rotation_matrix = Matrix4x4::getXRotationMatrix(this->p_pitch);
  const auto &y_rotation_matrix = Matrix4x4::getYRotationMatrix(this->p_yaw);

  multiplyForNewDirection(this->p_direction, this->p_target, x_rotation_matrix,
                          y_rotation_matrix);
}

void Camera::moveRight(const double &theta) {
  const auto up_vector = Vec3D(0, 1, 0);

  auto right_vector = this->p_direction.getCrossProduct(up_vector).normalize();
  this->p_position = this->p_position + (right_vector * STEP * theta);
}

void Camera::turnRight(const double &theta) {
  this->p_yaw += STEP * 0.5f * theta;

  const auto &x_rotation_matrix = Matrix4x4::getXRotationMatrix(this->p_pitch);
  const auto &y_rotation_matrix = Matrix4x4::getYRotationMatrix(this->p_yaw);

  multiplyForNewDirection(this->p_direction, this->p_target, x_rotation_matrix,
                          y_rotation_matrix);
}

void Camera::turnUp(const double &theta) {
  this->p_pitch = std::fmin(
      std::fmax(this->p_pitch - (STEP * 0.5f * theta), -this->p_max_rotation),
      this->p_max_rotation);

  const auto &x_rotation_matrix = Matrix4x4::getXRotationMatrix(this->p_pitch);
  const auto &y_rotation_matrix = Matrix4x4::getYRotationMatrix(this->p_yaw);

  multiplyForNewDirection(this->p_direction, this->p_target, x_rotation_matrix,
                          y_rotation_matrix);
}

void Camera::turnDown(const double &theta) {
  this->p_pitch = std::fmin(
      std::fmax(this->p_pitch + (STEP * 0.5f * theta), -this->p_max_rotation),
      this->p_max_rotation);

  const auto &x_rotation_matrix = Matrix4x4::getXRotationMatrix(this->p_pitch);
  const auto &y_rotation_matrix = Matrix4x4::getYRotationMatrix(this->p_yaw);

  multiplyForNewDirection(this->p_direction, this->p_target, x_rotation_matrix,
                          y_rotation_matrix);
}

void Camera::cameraTurn(const double &theta, const double &new_cursor_x_pos,
                        const double &new_cursor_y_pos) {
  const auto x_difference = new_cursor_x_pos - this->p_x_cursor_prev_pos;
  this->p_x_cursor_prev_pos = new_cursor_x_pos;

  const auto y_difference = new_cursor_y_pos - this->p_y_cursor_prev_pos;
  this->p_y_cursor_prev_pos = new_cursor_y_pos;

  if (x_difference == 0 && y_difference == 0) {
    return;
  }

  // for the x rotation we rotated across the y axis
  this->p_pitch =
      std::fmin(std::fmax(this->p_pitch + (MOUSE_SPEED * theta * y_difference),
                          -this->p_max_rotation),
                this->p_max_rotation);

  // for the y rotation we rotate across the x axis
  this->p_yaw += MOUSE_SPEED * theta * x_difference;

  const auto &x_rotation_matrix = Matrix4x4::getXRotationMatrix(this->p_pitch);
  const auto &y_rotation_matrix = Matrix4x4::getYRotationMatrix(this->p_yaw);

  multiplyForNewDirection(this->p_direction, this->p_target, x_rotation_matrix,
                          y_rotation_matrix);
}

const Matrix4x4 Camera::getLookAtMatrix() const {
  auto forward = this->p_direction.normalize();

  auto up_vector = Vec3D(0, 1, 0);

  // if my direction is straight up/down
  //  use z as the up vector
  if (fabs(forward.getDotProduct(up_vector)) > 0.999f) {
    up_vector = Vec3D(0, 0, 1);
  }

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
