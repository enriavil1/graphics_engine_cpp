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

private:
  static std::vector<std::shared_ptr<Object3D>> p_loaded_objects;

  static Mesh p_projecting_mesh;
  static int getAmountOfTrianglesProjecting();

  // TODO: Actually make a camera not just a point
  static Vec3D camera;

  static void multiplyVectorMatrix(const Vec3D &point, Vec3D &output,
                                   const Matrix4x4 &m);

  static void scaleTriangle(Triangle &triangle);
};

} // namespace engine3D
