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

  Matrix4x4 getLookAtMatrix();

  double getNear();
  double getFar();

  Vec3D getPos();
  Vec3D getDirection();

private:
  Vec3D p_position = Vec3D(0, 0, 0);
  Vec3D p_direction = Vec3D(0, 0, 1);

  double p_y_rotation = 0.0;

  double p_near = 0.1f;
  double p_far = 1.0f;
};

}; // namespace engine3D
