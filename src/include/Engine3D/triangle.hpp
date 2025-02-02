#pragma once

#include <array>
#include <vector>

#include "vec3d.hpp"

namespace engine3D {

class Triangle {
public:
  std::array<Vec3D, 3> points;

  Triangle() {};
  Triangle(std::array<Vec3D, 3> points) : points(points) {};

  Vec3D getNormarl() {
    // A line is a length between two points
    Vec3D lineA = Vec3D(points[1].x - points[0].x, points[1].y - points[0].y,
                        points[1].z - points[0].z);

    Vec3D lineB = Vec3D(points[2].x - points[0].x, points[2].y - points[0].y,
                        points[2].z - points[0].z);

    Vec3D normal = Vec3D(lineA.y * lineB.z - lineA.z * lineB.y,
                         lineA.z * lineB.x - lineA.x * lineB.z,
                         lineA.x * lineB.y - lineA.y * lineB.x);

    // normalize the normal
    float normalizer =
        sqrtf(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

    normal.x /= normalizer;
    normal.y /= normalizer;
    normal.z /= normalizer;

    return normal;
  };
};

// TODO move to its own file and turn into a class
struct Mesh {
  std::vector<Triangle> triangles;
};

} // namespace engine3D
