#pragma once

#include <array>
#include <vector>

#include "vec3d.hpp"

namespace engine3D {

// TODO: Create cpp file for this
class Triangle {
public:
  std::array<ImU32, 3> colors = {IM_COL32_WHITE, IM_COL32_WHITE,
                                 IM_COL32_WHITE};
  std::array<Vec3D, 3> points;

  Triangle() {};
  Triangle(std::array<Vec3D, 3> points) : points(points) {};

  Vec3D getNormarl() {
    // A line is a length between two points
    Vec3D lineA = points[1] - points[0];
    Vec3D lineB = points[2] - points[0];

    auto normal = lineA.getCrossProduct(lineB);

    // normalize the normal
    return normal.normalize();
  };
  ;
};

// TODO move to its own file and turn into a class
struct Mesh {
  std::vector<Triangle> triangles;
};

} // namespace engine3D
