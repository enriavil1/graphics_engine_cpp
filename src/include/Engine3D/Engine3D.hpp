#pragma once

#include "point.hpp"

namespace engine3D {

struct Matrix4x4 {
  float m[4][4] = {0};
};

class Engine {
public:
  inline Engine() {
    this->p_mesh_cube.triangles = {
        // South
        {ImVec4(0, 0, 0, 0), ImVec4(0, 1, 0, 0), ImVec4(1, 1, 0, 0)},
        {ImVec4(0, 0, 0, 0), ImVec4(1, 1, 0, 0), ImVec4(1, 0, 0, 0)},

        // East
        {ImVec4(1, 0, 0, 0), ImVec4(1, 1, 0, 0), ImVec4(1, 1, 1, 0)},
        {ImVec4(1, 0, 0, 0), ImVec4(1, 1, 1, 0), ImVec4(1, 0, 1, 0)},

        // North
        {ImVec4(1, 0, 1, 0), ImVec4(1, 1, 1, 0), ImVec4(0, 1, 1, 0)},
        {ImVec4(1, 0, 1, 0), ImVec4(0, 1, 1, 0), ImVec4(0, 0, 1, 0)},

        // West
        {ImVec4(0, 0, 1, 0), ImVec4(0, 1, 1, 0), ImVec4(0, 1, 0, 0)},
        {ImVec4(0, 0, 1, 0), ImVec4(0, 1, 0, 0), ImVec4(0, 0, 0, 0)},

        // Top
        {ImVec4(0, 1, 0, 0), ImVec4(0, 1, 1, 0), ImVec4(1, 1, 1, 0)},
        {ImVec4(0, 1, 0, 0), ImVec4(1, 1, 1, 0), ImVec4(1, 1, 0, 0)},

        // Bottom
        {ImVec4(1, 0, 1, 0), ImVec4(0, 0, 1, 0), ImVec4(0, 0, 0, 0)},
        {ImVec4(1, 0, 1, 0), ImVec4(0, 0, 0, 0), ImVec4(1, 0, 0, 0)},
    };
  };

  inline ~Engine() { this->p_mesh_cube.triangles = {}; };

  void project(double theta);

private:
  Mesh p_mesh_cube;

  void multiplyVectorMatrix(const ImVec4 &point, ImVec4 &output,
                            const Matrix4x4 &m);
};

} // namespace engine3D
