#pragma once

#include "matrix4x4/matrix4x4.hpp"
#include "vec3d.hpp"

namespace engine3D {
class Camera {
public:
  Camera(double near, double far) : p_near(near), p_far(far) {};

  void moveForward(const double &theta);
  void moveBackwards(const double &theta);
  void moveLeft(const double &theta);
  void moveRight(const double &theta);
  void moveUp(const double &theta);
  void moveDown(const double &theta);

  void turnLeft(const double &theta);
  void turnRight(const double &theta);
  void turnUp(const double &theta);
  void turnDown(const double &theta);

  // Specific turn to be done by the camera
  // turning x and y axis at the same time
  void cameraTurn(const double &theta, const double &new_cursor_x_pos,
                  const double &new_cursor_y_pos);

  Matrix4x4 getLookAtMatrix();

  double getNear();
  double getFar();

  Vec3D getPos();
  Vec3D getDirection();

private:
  Vec3D p_position = Vec3D(0, 0, 0);
  Vec3D p_direction = Vec3D(0, 0, 1);

  const Vec3D p_target = Vec3D(0, 0, 1);

  double p_y_rotation = 0.0;
  double p_x_rotation = 0.0;

  double p_near = 0.1f;
  double p_far = 1.0f;

  double p_x_cursor_prev_pos = 0.0;
  double p_y_cursor_prev_pos = 0.0;
};

}; // namespace engine3D
