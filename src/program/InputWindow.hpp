#pragma once


#include "prime/CFinalInput.hpp"
#include "prime/CPlayerMP1.hpp"
#include <vector>
#include <cstdint>

namespace GUI {
  extern int framesSincePlayerInput;
  extern CFinalInput lastKnownCPlayerInput;
  extern double igt;

  extern EPlayerMovementState moveState;
  extern uint32_t bombJumpState;
  extern uint32_t isInHalfPipeMode;
  extern uint32_t isInHalfPipeModeInAir;
  extern uint32_t touchedHalfPipeRecently;

  void drawInputWindow();
  void drawMonitorWindow();
}