#pragma once

#include "triangle.hpp"

namespace engine3D {

typedef float Matrix4x4[4][4];

class Engine {
public:
  inline Engine() {
    this->p_mesh_cube.triangles = {
        // South
        Triangle({Point(0, 0, 0), Point(0, 1, 0), Point(1, 1, 0)}),
        Triangle({Point(0, 0, 0), Point(1, 1, 0), Point(1, 0, 0)}),

        // East
        Triangle({Point(1, 0, 0), Point(1, 1, 0), Point(1, 1, 1)}),
        Triangle({Point(1, 0, 0), Point(1, 1, 1), Point(1, 0, 1)}),

        // North
        Triangle({Point(1, 0, 1), Point(1, 1, 1), Point(0, 1, 1)}),
        Triangle({Point(1, 0, 1), Point(0, 1, 1), Point(0, 0, 1)}),

        // West
        Triangle({Point(0, 0, 1), Point(0, 1, 1), Point(0, 1, 0)}),
        Triangle({Point(0, 0, 1), Point(0, 1, 0), Point(0, 0, 0)}),

        // Top
        Triangle({Point(0, 1, 0), Point(0, 1, 1), Point(1, 1, 1)}),
        Triangle({Point(0, 1, 0), Point(1, 1, 1), Point(1, 1, 0)}),

        // Bottom
        Triangle({Point(1, 0, 1), Point(0, 0, 1), Point(0, 0, 0)}),
        Triangle({Point(1, 0, 1), Point(0, 0, 0), Point(1, 0, 0)}),
    };
  };

  inline ~Engine() { this->p_mesh_cube.triangles = {}; };

  void project(double theta);

private:
  Mesh p_mesh_cube;
  // Actually make a camera not just a point
  Point camera;

  void multiplyVectorMatrix(const Point &point, Point &output,
                            const Matrix4x4 &m);

  void scaleTriangle(Triangle &triangle);
};

} // namespace engine3D
