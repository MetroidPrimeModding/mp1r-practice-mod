#pragma once

#include "prime/Math.hpp"
#include "prime/CFirstPersonCamera.hpp"

namespace GUI {
  extern bool is_time_stopped;
  extern float verticalFov;
  extern bool shouldUpdateVerticalFov;
  extern float viewRoll;
  extern CGameCameraMP1* fpCamera;
  extern float exposure;

  const bool shouldHideAll();
  void drawPhotoModeMenu();
} // namespace GUI