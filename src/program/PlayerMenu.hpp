#pragma once

#include "prime/Math.hpp"

namespace GUI {
  extern bool hasDesiredPositionData;

  extern CTransform4f desiredTransform;
  extern CTransform4f lastKnownTransform;

  extern CVector3f desiredVelocity;
  extern CVector3f lastKnownVelocity;

  extern CAxisAngle desiredAngularVelocity;
  extern CAxisAngle lastKnownAngularVelocity;

  extern double desiredTime;

  void drawPlayerMenu();
  void savePos();
  void loadPos();
}