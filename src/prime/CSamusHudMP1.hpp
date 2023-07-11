#pragma once

namespace rstl {
  template <class T, typename storage, T val>
  struct enum_bit_field;
};

class CStateManager;

class CGuiCamera;

class CAutoMapperMP1 {
public:
  void Update(float f1, const CStateManager &csm, const CGuiCamera& cgc, float f2, bool b1);
};


class CSamusHudMP1 {
public:
  enum class EHudDrawFilter : uint32_t;

  void Draw(const CStateManager &stateManager, float alpha, uint32_t param_3, const rstl::enum_bit_field<CSamusHudMP1::EHudDrawFilter, unsigned int, (CSamusHudMP1::EHudDrawFilter)3> &hudDrawFilterFlags) const;
  void DrawHelmet(const CStateManager& stateManager, float camYOffset) const;
  void DrawTargetingReticle(const CStateManager&) const;
};
