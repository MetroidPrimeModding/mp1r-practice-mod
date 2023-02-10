#include "patches.hpp"
#include "logger/Logger.hpp"
#include "prime/CObjectId.hpp"
#include "InventoryMenu.hpp"
#include "prime/CPlayerMP1.hpp"
#include "prime/CFinalInput.hpp"
#include "PlayerMenu.hpp"
#include "InputWindow.hpp"

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

CStateManager *mostRecentStateManager;

HOOK_DEFINE_TRAMPOLINE(CPlayer_ProcessInput) {
  static void Callback(CPlayerMP1 *thiz, const CFinalInput &input, CStateManager &stateManager) {
    if (GUI::hasDesiredPositionData) {
      GUI::hasDesiredPositionData = false;
      thiz->SetTransform(GUI::desiredTransform);
      thiz->SetVelocityWR(stateManager, GUI::desiredVelocity);
      thiz->SetAngularVelocityWR(stateManager, GUI::desiredAngularVelocity);
    }

    Orig(thiz, input, stateManager);

    GUI::lastKnownTransform = thiz->GetTransform();
    GUI::lastKnownVelocity = thiz->GetVelocityWR(stateManager);
    GUI::lastKnownAngularVelocity = thiz->GetAngularVelocityWR(stateManager);
//    GUI::lastKnownInput = input;
//    GUI::hasInput = true;
  }
};

HOOK_DEFINE_TRAMPOLINE(CStateManagerGameLogicMP1_GameMainLoop) {
  static void Callback(void *thiz, CStateManager& mgr, void *updateAccess, float dt) {
    Orig(thiz, mgr, updateAccess, dt);

    GUI::lastKnownInput = *(CFinalInput *)((size_t)thiz + 0x160);
    GUI::hasInput = true;
  }
};

void runCodePatches() {
  Dash_ScanVisorCheck::InstallAtOffset(0xceeab0ull);
  Dash_ScanVisorCheck::InstallAtOffset(0xceeac4ull);
  Dash_Speed::InstallAtOffset(0xcef7a8ull);
  Dash_VelocityClamp::InstallAtOffset(0xcee31Cull);
  Dash_VelocityClamp::InstallAtOffset(0xcee334ull);

  // Uncomment this hook to cause physics to be ice
//  CheckFloatVar::InstallAtOffset(0xcee418ll);


  CStateManager_DoThinkLogic::InstallAtSymbol("_ZN13CStateManager12DoThinkLogicEf");
  CPlayer_ProcessInput::InstallAtOffset(0xc70334ull);
  CStateManagerGameLogicMP1_GameMainLoop::InstallAtOffset(0xc5bc4cull);
}