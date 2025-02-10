#include "../../include/Engine3D/vec3d.hpp"
#include <cmath>

using namespace engine3D;

Vec3D Vec3D::operator+(const Vec3D &vec3d) {
  return Vec3D(this->x + vec3d.x, this->y + vec3d.y, this->z + vec3d.z);
}

Vec3D Vec3D::operator-(const Vec3D &vec3d) {
  return Vec3D(this->x - vec3d.x, this->y - vec3d.y, this->z - vec3d.z);
}

Vec3D Vec3D::operator*(const Vec3D &vec3d) {
  return Vec3D(this->x * vec3d.x, this->y * vec3d.y, this->z * vec3d.z);
}

Vec3D Vec3D::operator/(const Vec3D &vec3d) {
  return Vec3D(this->x / vec3d.x, this->y / vec3d.y, this->z / vec3d.z);
}

float Vec3D::getDotProduct(const Vec3D &vec3d) {
  return this->x * vec3d.x + this->y * vec3d.y + this->z * vec3d.z;
}

float Vec3D::getVectorLength(const Vec3D &vec3d) {
  return sqrtf(this->getDotProduct(vec3d));
}

Vec3D Vec3D::getCrossProduct(const Vec3D &vec3d) {
  return Vec3D(this->y * vec3d.z - this->z * vec3d.y,
               this->z * vec3d.x - this->x * vec3d.z,
               this->x * vec3d.y - this->y * vec3d.x);
}

Vec3D Vec3D::normalize(const Vec3D &vec3d) {
  const auto vector_length = this->getVectorLength(vec3d);
  return Vec3D(this->x / vector_length, this->y / vector_length,
               this->z / vector_length);
}
