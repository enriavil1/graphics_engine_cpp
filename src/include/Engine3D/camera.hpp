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

private:
  Vec3D position = Vec3D();
};

}; // namespace engine3D
