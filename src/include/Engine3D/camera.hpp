#pragma once

#include "matrix4x4/matrix4x4.hpp"
#include "vec3d.hpp"

namespace engine3D {
class Camera {
public:
  Camera() {};

  void moveForward();
  void moveBackwards();
  void moveLeft();
  void moveRight();
  void moveUp();
  void moveDown();

  void turnLeft();
  void turnRight();

  Matrix4x4 getLookAtMatrix();

  Vec3D getPos();
  Vec3D getDirection();

  Vec3D up_vector = Vec3D(0, 1, 0);

private:
  Vec3D p_position = Vec3D(0, 0, 0);
  Vec3D p_direction = Vec3D(0, 0, 1);

  double p_y_rotation = 0.0;
};

}; // namespace engine3D
