#pragma once

#include "program/GetField.hpp"
#include "prime/CActorMP1.hpp"

class CStateManager;

class CGameCamera : public CActorMP1 {};

class CGameCameraMP1: public CGameCamera {
public:
  inline float* GetCurrentFOV() {
      return GetField<float>(this, 0x25c);
  };
  void UpdateFOV(float dt);
};

class CFirstPersonCameraMP1 : public CGameCameraMP1 {
public:
  void Think(float, CStateManager&);
};

class CCameraManagerMP1 {
public:
  static float GetDefaultFirstPersonVerticalFOV();
};

class STonemapParams {
public:
  inline float* inverseExposure() { return GetField<float>(this, 0x0); };
};

namespace NTonemap {
  void build_tonemap_eval_params(STonemapParams const&);
}