#pragma once

#include "prime/Math.hpp"
#include "types.h"

namespace GUI {
  extern bool hasDesiredPositionData;
  extern bool hasDesiredTimeData;

  extern CTransform4f desiredTransform;
  extern CTransform4f lastKnownTransform;

  extern CVector3f desiredVelocity;
  extern CVector3f lastKnownVelocity;

  extern CAxisAngle desiredAngularVelocity;
  extern CAxisAngle lastKnownAngularVelocity;

  extern double desiredTime;

  extern bool isInCutscene;
  extern bool alive;
  extern bool kill;
  extern int32_t loadPhase;

  void drawPlayerMenu();
  void savePos();
  void loadPos();

  void saveTime();
  void loadTime();
}