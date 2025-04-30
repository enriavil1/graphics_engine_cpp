#pragma once

#include "camera.hpp"
#include "matrix4x4/matrix4x4.hpp"
#include "object3D.hpp"
#include "triangle.hpp"
#include "vec3d.hpp"

#include <string>

#define NEAR 0.1
#define FAR 1.0

namespace engine3D {

class Engine {
public:
  inline static bool show_wire_frame = false;

  static void project(double theta);
  static bool loadObject(std::string file_path);
  static bool getProjectingObj(Object3D &obj);

  static Camera &getCamera();

private:
  inline static std::vector<std::shared_ptr<Object3D>> mp_loaded_objects = {};

  inline static std::shared_ptr<Object3D> mp_projecting_obj = nullptr;

  inline static Camera mp_camera = Camera(NEAR, FAR);

  static void multiplyVectorMatrix(const Vec3D &point, Vec3D &output,
                                   const Matrix4x4 &m);

  static Vec3D getPlaneInterception(const Vec3D &point_on_plane,
                                    const Vec3D &plane,
                                    const Vec3D &start_of_line,
                                    const Vec3D &end_of_line);
  static int clipTriangle(const Vec3D &plane_a, const Vec3D &plane_b,
                          Triangle &triangle,
                          std::vector<Triangle> &triangles_output);

  static void scaleTriangle(Triangle &triangle);
  static void scaleVec2d(ImVec2 &point);
  static void scaleVec2d(Vec3D &point);
};

} // namespace engine3D
