#pragma once

#include "triangle.hpp"

namespace engine3D {
class Object3D {
private:
  Mesh p_mesh;
  int p_amount_of_vertices;
  const char *p_name;

public:
  Object3D(Mesh mesh, int amount_of_vertices, const char *name)
      : p_mesh(mesh), p_amount_of_vertices(amount_of_vertices), p_name(name) {};

  int getAmountOfVertices();
  Mesh getMesh();
  const char *getObjectName();
};
} // namespace engine3D
