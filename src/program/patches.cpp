#include "patches.hpp"
#include "logger/Logger.hpp"
#include "CObject/CObjectId.h"
#include "InventoryMenu.hpp"

namespace patch = exl::patch;
namespace inst = exl::armv8::inst;
namespace reg = exl::armv8::reg;

PatchConfig PATCH_CONFIG{};

HOOK_DEFINE_INLINE(Dash_ScanVisorCheck) {
  static void Callback(exl::hook::InlineCtx *ctx) {
    if (PATCH_CONFIG.dash_enabled) {
      ctx->W[8] = 0;
    }
  }
};

HOOK_DEFINE_INLINE(Dash_Speed) {
  static void Callback(exl::hook::InlineCtx *ctx) {
    if (PATCH_CONFIG.dash_enabled) {
      ctx->W[8] = 0;
    }
  }
};

HOOK_DEFINE_INLINE(Dash_VelocityClamp) {
  static void Callback(exl::hook::InlineCtx *ctx) {
    if (PATCH_CONFIG.dash_enabled) {
      ctx->W[8] = 0;
    }
  }
};

HOOK_DEFINE_INLINE(CheckFloatVar) {
  static void Callback(exl::hook::InlineCtx *ctx) {
    // This hook (even if empty) casuses physics to be ice
//    Logger::log("CheckFloatVar: %f\n", ctx->S[1], offsetof(exl::hook::InlineCtx, m_Fpr));
  }
};

HOOK_DEFINE_TRAMPOLINE(CStateManager_DoThinkLogic) {
  static void Callback(void *arg1, float arg2) {
    mostRecentStateManager = (CStateManager *) arg1;
    return Orig(arg1, arg2);
  }
};
CStateManager* mostRecentStateManager;

void runCodePatches() {
  Dash_ScanVisorCheck::InstallAtOffset(0xceeab0ull);
  Dash_ScanVisorCheck::InstallAtOffset(0xceeac4ull);
  Dash_Speed::InstallAtOffset(0xcef7a8ull);
  Dash_VelocityClamp::InstallAtOffset(0xcee31Cull);
  Dash_VelocityClamp::InstallAtOffset(0xcee334ull);

  // Uncomment this hook to cause physics to be ice
//  CheckFloatVar::InstallAtOffset(0xcee418ll);


  CStateManager_DoThinkLogic::InstallAtSymbol("_ZN13CStateManager12DoThinkLogicEf");
//  CStateManager_DoThinkLogic::InstallAtOffset(0xb03678ull);
}