#pragma once

#include <vector>

#include "triangle.hpp"
#include "vec3d.hpp"

namespace engine3D {
class Object3D {
private:
  std::vector<Vec3D> p_vertices;
  Mesh p_mesh;
  const char *p_name;

public:
  Object3D(std::vector<Vec3D> vertices, Mesh mesh, const char *name);

  std::vector<Vec3D> getVertices();
  Mesh getMesh();
};
} // namespace engine3D
