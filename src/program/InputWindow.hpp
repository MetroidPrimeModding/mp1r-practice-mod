#pragma once


#include "prime/CFinalInput.hpp"

namespace GUI {
  extern bool hasInput;
  extern CFinalInput lastKnownInput;
  extern double igt;

  void drawInputWindow();
  void drawMonitorWindow();
}