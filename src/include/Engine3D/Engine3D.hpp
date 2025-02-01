#pragma once

#include "point.hpp"

namespace engine3D {

typedef float Matrix4x4[4][4];

class Engine {
public:
  inline Engine() {
    this->p_mesh_cube.triangles = {
        // South
        {Point(0, 0, 0), Point(0, 1, 0), Point(1, 1, 0)},
        {Point(0, 0, 0), Point(1, 1, 0), Point(1, 0, 0)},

        // East
        {Point(1, 0, 0), Point(1, 1, 0), Point(1, 1, 1)},
        {Point(1, 0, 0), Point(1, 1, 1), Point(1, 0, 1)},

        // North
        {Point(1, 0, 1), Point(1, 1, 1), Point(0, 1, 1)},
        {Point(1, 0, 1), Point(0, 1, 1), Point(0, 0, 1)},

        // West
        {Point(0, 0, 1), Point(0, 1, 1), Point(0, 1, 0)},
        {Point(0, 0, 1), Point(0, 1, 0), Point(0, 0, 0)},

        // Top
        {Point(0, 1, 0), Point(0, 1, 1), Point(1, 1, 1)},
        {Point(0, 1, 0), Point(1, 1, 1), Point(1, 1, 0)},

        // Bottom
        {Point(1, 0, 1), Point(0, 0, 1), Point(0, 0, 0)},
        {Point(1, 0, 1), Point(0, 0, 0), Point(1, 0, 0)},
    };
  };

  inline ~Engine() { this->p_mesh_cube.triangles = {}; };

  void project(double theta);

private:
  Mesh p_mesh_cube;
  Point camera = {.x = 0, .y = 0, .z = 0};

  void multiplyVectorMatrix(const Point &point, Point &output,
                            const Matrix4x4 &m);

  void scaleTriangle(Triangle &triangle);
};

} // namespace engine3D
