#include "../../include/Engine3D/vec3d.hpp"
#include <cmath>

using namespace engine3D;

Vec3D Vec3D::operator+(const Vec3D &vec3d) const {
  return Vec3D(this->x + vec3d.x, this->y + vec3d.y, this->z + vec3d.z);
}

Vec3D Vec3D::operator-(const Vec3D &vec3d) const {
  return Vec3D(this->x - vec3d.x, this->y - vec3d.y, this->z - vec3d.z);
}

Vec3D Vec3D::operator*(const Vec3D &vec3d) const {
  return Vec3D(this->x * vec3d.x, this->y * vec3d.y, this->z * vec3d.z);
}

Vec3D Vec3D::operator*(const float &num) const {
  return Vec3D(this->x * num, this->y * num, this->z * num);
}

Vec3D Vec3D::operator*(const Matrix4x4 &matrix) const {
  Vec3D output;

  output.x = this->x * matrix[0][0] + this->y * matrix[1][0] +
             this->z * matrix[2][0] + this->w * matrix[3][0];

  output.y = this->x * matrix[0][1] + this->y * matrix[1][1] +
             this->z * matrix[2][1] + this->w * matrix[3][1];

  output.z = this->x * matrix[0][2] + this->y * matrix[1][2] +
             this->z * matrix[2][2] + this->w * matrix[3][2];

  output.w = this->x * matrix[0][3] + this->y * matrix[1][3] +
             this->z * matrix[2][3] + this->w * matrix[3][3];

  return output;
}

Vec3D Vec3D::operator/(const Vec3D &vec3d) const {
  return Vec3D(this->x / vec3d.x, this->y / vec3d.y, this->z / vec3d.z);
}

Vec3D Vec3D::operator/(const float &num) const {
  return Vec3D(this->x / num, this->y / num, this->z / num);
}

float Vec3D::getDotProduct(const Vec3D &vec3d) const {
  return this->x * vec3d.x + this->y * vec3d.y + this->z * vec3d.z;
}

float Vec3D::getDotProduct() const {
  return this->x * this->x + this->y * this->y + this->z * this->z;
}

float Vec3D::getVectorLength() const { return sqrtf(this->getDotProduct()); }

Vec3D Vec3D::getCrossProduct(const Vec3D &vec3d) const {
  return Vec3D(this->y * vec3d.z - this->z * vec3d.y,
               this->z * vec3d.x - this->x * vec3d.z,
               this->x * vec3d.y - this->y * vec3d.x);
}

Vec3D Vec3D::normalize() const {
  const auto vector_length = this->getVectorLength();

  // avoid division by 0
  if (vector_length == 0)
    return Vec3D(0, 0, 0);

  return Vec3D(this->x / vector_length, this->y / vector_length,
               this->z / vector_length);
}
