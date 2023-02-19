#include "patches.hpp"
#include "logger/Logger.hpp"
#include "prime/CObjectId.hpp"
#include "InventoryMenu.hpp"
#include "prime/CPlayerMP1.hpp"
#include "prime/CFinalInput.hpp"
#include "PlayerMenu.hpp"
#include "InputWindow.hpp"
#include "helpers/InputHelper.h"

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

CStateManager *mostRecentStateManager;

HOOK_DEFINE_TRAMPOLINE(CPlayer_ProcessInput) {
  static void Callback(CPlayerMP1 *thiz, const CFinalInput &input, CStateManager &stateManager) {
    mostRecentStateManager = &stateManager;
    auto *gameState = stateManager.GameState();
    GUI::igt = gameState->GetPlayTime();

    GUI::moveState = thiz->GetMoveState();
    GUI::bombJumpState = thiz->GetMorphBall()->GetBombJumpState();
    GUI::isInHalfPipeMode = thiz->GetMorphBall()->GetIsInHalfPipeMode();
    GUI::isInHalfPipeModeInAir = thiz->GetMorphBall()->GetIsInHalfPipeModeInAir();
    GUI::touchedHalfPipeRecently = thiz->GetMorphBall()->GetTouchedHalfPipeRecently();

    if (GUI::hasDesiredPositionData) {
      GUI::hasDesiredPositionData = false;
      thiz->SetTransform(GUI::desiredTransform);
      thiz->SetVelocityWR(stateManager, GUI::desiredVelocity);
      thiz->SetAngularVelocityWR(stateManager, GUI::desiredAngularVelocity);
    } else {
      if (!InputHelper::isInputToggled()) {
        if (input.GetDigitalHeld(EControl::ZR) && input.GetDigitalHeld(EControl::R) && input.GetDigitalHeld(EControl::DPAD_UP)) {
          GUI::loadPos();
        }
        if (input.GetDigitalHeld(EControl::RIGHT_STICK_CLICK) && input.GetDigitalHeld(EControl::DPAD_UP)) {
          GUI::loadPos();
        }
        if (input.GetDigitalHeld(EControl::ZR) && input.GetDigitalHeld(EControl::R) && input.GetDigitalHeld(EControl::DPAD_DOWN)) {
          GUI::savePos();
        }
        if (input.GetDigitalHeld(EControl::RIGHT_STICK_CLICK) && input.GetDigitalHeld(EControl::DPAD_DOWN)) {
          GUI::savePos();
        }
      } else {
        if (PATCH_CONFIG.pos_edit && InputHelper::isHoldZR()) {
          float multiplier = 0.3f;
          if (InputHelper::isHoldZL()) {
            multiplier = 0.9f;
          }
          CVector3f movement = CVector3f(
              -InputHelper::getLeftStickX() * multiplier,
              InputHelper::getRightStickY() * multiplier,
              InputHelper::getLeftStickY() * multiplier
          );
          CTransform4f transform = thiz->GetTransform();
          movement = transform.Rotate(movement);
//          Logger::log("PosD: %f, %f, %f\n", x,y,z);
//          Logger::log("Pos: %f, %f, %f\n", GUI::lastKnownTransform.x, GUI::lastKnownTransform.y, GUI::lastKnownTransform.z);
          GUI::lastKnownTransform.x = GUI::lastKnownTransform.x + movement.x;
          GUI::lastKnownTransform.y = GUI::lastKnownTransform.y + movement.y;
          GUI::lastKnownTransform.z = GUI::lastKnownTransform.z + movement.z;
//          Logger::log("Pos: %f, %f, %f\n", GUI::lastKnownTransform.x, GUI::lastKnownTransform.y, GUI::lastKnownTransform.z);

          auto rot = CRelAngle(InputHelper::getRightStickX() * -0.05f);
          CTransform4f rotT = CTransform4f::Identity();
          rotT.RotateLocalY(rot);
          GUI::lastKnownTransform = GUI::lastKnownTransform * rotT;
        }

        thiz->SetTransform(GUI::lastKnownTransform);
        thiz->SetVelocityWR(stateManager, GUI::lastKnownVelocity);
        thiz->SetAngularVelocityWR(stateManager, GUI::lastKnownAngularVelocity);
      }

    }

    Orig(thiz, input, stateManager);

    GUI::lastKnownTransform = thiz->GetTransform();
    GUI::lastKnownVelocity = *thiz->GetVelocityWR(stateManager);
    GUI::lastKnownAngularVelocity = *thiz->GetAngularVelocityWR(stateManager);
//    GUI::lastKnownInput = input;
//    GUI::hasInput = true;
  }
};

HOOK_DEFINE_TRAMPOLINE(CStateManagerGameLogicMP1_GameMainLoop) {
  static void Callback(void *thiz, CStateManager &mgr, void *updateAccess, float dt) {
    Orig(thiz, mgr, updateAccess, dt);

    GUI::lastKnownInput = *(CFinalInput *) ((size_t) thiz + 0x160);
    GUI::hasInput = true;
  }
};

void runCodePatches() {
//  Dash_ScanVisorCheck::InstallAtOffset(0xceeab0ull);
//  Dash_ScanVisorCheck::InstallAtOffset(0xceeac4ull);
//  Dash_Speed::InstallAtOffset(0xcef7a8ull);
//  Dash_VelocityClamp::InstallAtOffset(0xcee31Cull);
//  Dash_VelocityClamp::InstallAtOffset(0xcee334ull);

  // Uncomment this hook to cause physics to be ice
//  CheckFloatVar::InstallAtOffset(0xcee418ll);

  CPlayer_ProcessInput::InstallAtOffset(0xc70334ull);
  CStateManagerGameLogicMP1_GameMainLoop::InstallAtOffset(0xc5bc4cull);
}