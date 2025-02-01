#pragma once

#include "triangle.hpp"

namespace engine3D {

typedef float Matrix4x4[4][4];

class Engine {
public:
  inline Engine() {
    this->p_mesh_cube.triangles = {
        // South
        Triangle({Vec3d(0, 0, 0), Vec3d(0, 1, 0), Vec3d(1, 1, 0)}),
        Triangle({Vec3d(0, 0, 0), Vec3d(1, 1, 0), Vec3d(1, 0, 0)}),

        // East
        Triangle({Vec3d(1, 0, 0), Vec3d(1, 1, 0), Vec3d(1, 1, 1)}),
        Triangle({Vec3d(1, 0, 0), Vec3d(1, 1, 1), Vec3d(1, 0, 1)}),

        // North
        Triangle({Vec3d(1, 0, 1), Vec3d(1, 1, 1), Vec3d(0, 1, 1)}),
        Triangle({Vec3d(1, 0, 1), Vec3d(0, 1, 1), Vec3d(0, 0, 1)}),

        // West
        Triangle({Vec3d(0, 0, 1), Vec3d(0, 1, 1), Vec3d(0, 1, 0)}),
        Triangle({Vec3d(0, 0, 1), Vec3d(0, 1, 0), Vec3d(0, 0, 0)}),

        // Top
        Triangle({Vec3d(0, 1, 0), Vec3d(0, 1, 1), Vec3d(1, 1, 1)}),
        Triangle({Vec3d(0, 1, 0), Vec3d(1, 1, 1), Vec3d(1, 1, 0)}),

        // Bottom
        Triangle({Vec3d(1, 0, 1), Vec3d(0, 0, 1), Vec3d(0, 0, 0)}),
        Triangle({Vec3d(1, 0, 1), Vec3d(0, 0, 0), Vec3d(1, 0, 0)}),
    };
  };

  inline ~Engine() { this->p_mesh_cube.triangles = {}; };

  void project(double theta);

private:
  Mesh p_mesh_cube;
  // Actually make a camera not just a point
  Vec3d camera;

  void multiplyVectorMatrix(const Vec3d &point, Vec3d &output,
                            const Matrix4x4 &m);

  void scaleTriangle(Triangle &triangle);
};

} // namespace engine3D
