#include "../../include/Engine3D/camera.hpp"

#define STEP 8.0f

using namespace engine3D;

void Camera::moveForward() { this->p_position.z += STEP; }
void Camera::moveBackwards() { this->p_position.z -= STEP; }
void Camera::moveLeft() { this->p_position.x -= STEP; }
void Camera::moveRight() { this->p_position.x += STEP; }

Vec3D Camera::getPos() { return this->p_position; }
Vec3D Camera::getDirection() { return this->p_direction; }
