#pragma once

#include "vec3d.hpp"

namespace engine3D {
class Camera {
public:
  Camera() {};

  void moveForward();
  void moveBackwards();
  void moveLeft();
  void moveRight();

  Vec3D getPos();
  Vec3D getDirection();

private:
  Vec3D p_position = Vec3D(0, 0, 0);
  Vec3D p_direction = Vec3D(0, 0, 1);
};

}; // namespace engine3D
