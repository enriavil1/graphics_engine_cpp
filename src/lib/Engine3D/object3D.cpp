#include "../Engine3D/object3D.hpp"

using namespace engine3D;

Mesh Object3D::getMesh() { return this->p_mesh; }

int Object3D::getAmountOfVertices() { return this->p_amount_of_vertices; }

const char *Object3D::getObjectName() { return this->p_name; }
