#pragma once

#include <array>
#include <vector>

#include "point.hpp"

namespace engine3D {

class Triangle {
public:
  std::array<Point, 3> points;

  Triangle() {};
  Triangle(std::array<Point, 3> points) : points(points) {};

  Point getNormarl() {
    // A line is a length between two points
    Point lineA = Point(points[1].x - points[0].x, points[1].y - points[0].y,
                        points[1].z - points[0].z);

    Point lineB = Point(points[2].x - points[0].x, points[2].y - points[0].y,
                        points[2].z - points[0].z);

    Point normal = Point(lineA.y * lineB.z - lineA.z * lineB.y,
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
