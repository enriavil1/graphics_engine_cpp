#pragma once

#include "object3D.hpp"
#include "triangle.hpp"

#include <string>

namespace engine3D {

typedef float Matrix4x4[4][4];

class Engine {
public:
  static void project(double theta);
  static bool loadObject(std::string file_path);
  static bool getProjectingObj(Object3D &obj);

private:
  inline static std::vector<std::shared_ptr<Object3D>> p_loaded_objects = {};

  inline static std::shared_ptr<Object3D> p_projecting_obj = nullptr;

  // TODO: Actually make a camera not just a point
  inline static Vec3D camera = Vec3D();

  static void multiplyVectorMatrix(const Vec3D &point, Vec3D &output,
                                   const Matrix4x4 &m);

  static void scaleTriangle(Triangle &triangle);
};

} // namespace engine3D
