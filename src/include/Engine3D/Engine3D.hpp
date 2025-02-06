#pragma once

#include "object3D.hpp"
#include "triangle.hpp"

#include <string>

namespace engine3D {

typedef float Matrix4x4[4][4];

class Engine {
public:
  inline Engine() {
    this->p_projecting_mesh.triangles = {
        // South
        Triangle({Vec3D(0, 0, 0), Vec3D(0, 1, 0), Vec3D(1, 1, 0)}),
        Triangle({Vec3D(0, 0, 0), Vec3D(1, 1, 0), Vec3D(1, 0, 0)}),

        // East
        Triangle({Vec3D(1, 0, 0), Vec3D(1, 1, 0), Vec3D(1, 1, 1)}),
        Triangle({Vec3D(1, 0, 0), Vec3D(1, 1, 1), Vec3D(1, 0, 1)}),

        // North
        Triangle({Vec3D(1, 0, 1), Vec3D(1, 1, 1), Vec3D(0, 1, 1)}),
        Triangle({Vec3D(1, 0, 1), Vec3D(0, 1, 1), Vec3D(0, 0, 1)}),

        // West
        Triangle({Vec3D(0, 0, 1), Vec3D(0, 1, 1), Vec3D(0, 1, 0)}),
        Triangle({Vec3D(0, 0, 1), Vec3D(0, 1, 0), Vec3D(0, 0, 0)}),

        // Top
        Triangle({Vec3D(0, 1, 0), Vec3D(0, 1, 1), Vec3D(1, 1, 1)}),
        Triangle({Vec3D(0, 1, 0), Vec3D(1, 1, 1), Vec3D(1, 1, 0)}),

        // Bottom
        Triangle({Vec3D(1, 0, 1), Vec3D(0, 0, 1), Vec3D(0, 0, 0)}),
        Triangle({Vec3D(1, 0, 1), Vec3D(0, 0, 0), Vec3D(1, 0, 0)}),
    };
  };

  inline ~Engine() { this->p_projecting_mesh.triangles = {}; };

  void project(double theta);

  void loadObject(std::string file_path);

private:
  std::vector<std::shared_ptr<Object3D>> p_loaded_objects;

  Mesh p_projecting_mesh;

  // TODO: Actually make a camera not just a point
  Vec3D camera;

  void multiplyVectorMatrix(const Vec3D &point, Vec3D &output,
                            const Matrix4x4 &m);

  void scaleTriangle(Triangle &triangle);
};

} // namespace engine3D
