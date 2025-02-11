#include "../../include/Engine3D/camera.hpp"

#define STEP 8.0f

using namespace engine3D;

void Camera::moveForward() { this->position.z += STEP; }
void Camera::moveBackwards() { this->position.z -= STEP; }
void Camera::moveLeft() { this->position.x -= STEP; }
void Camera::moveRight() { this->position.x += STEP; }

Vec3D Camera::getPos() { return this->position; }
