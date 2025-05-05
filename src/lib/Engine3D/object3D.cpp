#include "../Engine3D/object3D.hpp"

using namespace engine3D;

Mesh Object3D::getMesh() const { return this->p_mesh; }
int Object3D::getAmountOfVertices() const { return this->p_amount_of_vertices; }

const std::string &Object3D::getObjectName() const { return this->p_name; }
