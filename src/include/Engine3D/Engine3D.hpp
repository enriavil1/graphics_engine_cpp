#pragma once

#include "camera.hpp"
#include "matrix4x4/matrix4x4.hpp"
#include "object3D.hpp"
#include "triangle.hpp"

#include <string>

namespace engine3D {

class Engine {
public:
  inline static bool show_wire_frame = false;

  static void project(double theta);
  static bool loadObject(std::string file_path);
  static bool getProjectingObj(Object3D &obj);

  static Camera &getCamera();

private:
  inline static std::vector<std::shared_ptr<Object3D>> p_loaded_objects = {};

  inline static std::shared_ptr<Object3D> p_projecting_obj = nullptr;

  inline static Camera p_camera = Camera();

  static void multiplyVectorMatrix(const Vec3D &point, Vec3D &output,
                                   const Matrix4x4 &m);

  static void scaleTriangle(Triangle &triangle);
};

} // namespace engine3D
