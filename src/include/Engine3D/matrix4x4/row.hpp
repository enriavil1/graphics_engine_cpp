#pragma once

#include <algorithm>

namespace engine3D {

// This is just a wrapper to allow for operator access for Matrix4x4
class Row {
private:
  float row[4] = {0};

public:
  Row() {};
  Row(float input_row[4]) {
    std::copy(&input_row[0], &input_row[0] + 4, &row[0]);
  }

  float operator[](const int &index) const { return this->row[index]; };
  float &operator[](const int &index) { return this->row[index]; };
};
} // namespace engine3D
