#pragma once

#include <string>

class FileDialog {
public:
  static std::string openDialog();

  inline static bool m_selecting_files = false;
};
