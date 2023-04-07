#include "patches.hpp"
#include "logger/Logger.hpp"
#include "prime/CObjectId.hpp"
#include "InventoryMenu.hpp"
#include "prime/CPlayerMP1.hpp"
#include "prime/CFinalInput.hpp"
#include "prime/CSamusHudMP1.hpp"
#include "PlayerMenu.hpp"
#include "InputWindow.hpp"
#include "helpers/InputHelper.h"
#include "imgui.h"
#include "nlohmann/json.hpp"
#include "helpers/fsHelper.h"
#include "init.h"

namespace patch = exl::patch;
namespace inst = exl::armv8::inst;
namespace reg = exl::armv8::reg;
using namespace nlohmann;

PatchConfig PATCH_CONFIG{};
CStateManager *mostRecentStateManager;


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

HOOK_DEFINE_TRAMPOLINE(CPlayerMP1_ProcessInput) {
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

    GUI::lastKnownCPlayerInput = input;
    GUI::framesSincePlayerInput = 0;
    GUI::lastKnownTransform = thiz->GetTransform();
    GUI::lastKnownVelocity = *thiz->GetVelocityWR(stateManager);
    GUI::lastKnownAngularVelocity = *thiz->GetAngularVelocityWR(stateManager);
//    GUI::lastKnownCPlayerInput = input;
//    GUI::framesSincePlayerInput = true;

    if (PATCH_CONFIG.invulnerable) {
      int etanks = CStateManagerGameLogicMP1::PlayerState()->GetItemCapacity(CPlayerStateMP1::EItemType::EnergyTanks);
      thiz->HealthInfo(stateManager).heatlh = (float)etanks * 100.0f + 99.0f;
    }
  }
};

HOOK_DEFINE_TRAMPOLINE(CSamusHudMP1_DrawTargetingReticle) {
  static void Callback(const CSamusHudMP1* self, const CStateManager& mgr) {
    if (!PATCH_CONFIG.hide_reticle) {
      Orig(self, mgr);
    }
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

  CPlayerMP1_ProcessInput::InstallAtFuncPtr(&CPlayerMP1::ProcessInput);
  CSamusHudMP1_DrawTargetingReticle::InstallAtFuncPtr(&CSamusHudMP1::DrawTargetingReticle);
}

void PatchConfig::loadConfig() {
  FsHelper::LoadData loadData = {
      .path = "sd:/mp1r/settings.json"
  };

  if (FsHelper::tryLoadFileFromPath(loadData)) {
    Logger::log("Loaded settings.json\n");

    std::string jsonStr((const char*)loadData.buffer, loadData.bufSize);
    json save = json::parse(jsonStr, nullptr, false);
    if (!save.is_discarded()) {
      loadFromJson(save);
    } else {
      Logger::log("Failed to parse settings.json\n");
    }
    nn::init::GetAllocator()->Free(loadData.buffer);
  } else {
    Logger::log("Failed to load settings.json\n");
  }
}

void PatchConfig::saveConfig() {
  json save = createSaveJson();

  std::string dump = save.dump(2);

  if (FsHelper::createDirectory("sd:/mp1r/") != 0) {
    this->requestConfigSave = false;
    return;
  }

  bool saved = FsHelper::writeFileToPath(dump.c_str(), dump.length(), "sd:/mp1r/settings.json");
  if (saved != 0) {
    Logger::log("Failed to save settings.json\n");
  }
  requestConfigSave = false;
}

void PatchConfig::loadFromJson(const json &json) {
  dash_enabled = json["dash_enabled"];
  pos_edit = json["pos_edit"];

  OSD_showInput = json.value("OSD_showInput", OSD_showInput);
  OSD_inputScale = json.value("OSD_inputScale", OSD_inputScale);

  OSD_showMonitor = json.value("OSD_showMonitor", OSD_showMonitor);
  OSD_showIGT = json.value("OSD_showIGT", OSD_showIGT);
  OSD_showPos = json.value("OSD_showPos", OSD_showPos);
  OSD_showVelocity = json.value("OSD_showVelocity", OSD_showVelocity);
  OSD_showMoveState = json.value("OSD_showMoveState", OSD_showMoveState);

  menuX = json.value("menuX", menuX);
  menuY = json.value("menuY", menuY);

  hide_reticle = json.value("hide_reticle", false);
}


json PatchConfig::createSaveJson() {
  json save;
  save["dash_enabled"] = dash_enabled;
  save["pos_edit"] = pos_edit;

  save["OSD_showInput"] = OSD_showInput;
  save["OSD_inputScale"] = OSD_inputScale;

  save["OSD_showMonitor"] = OSD_showMonitor;
  save["OSD_showIGT"] = OSD_showIGT;
  save["OSD_showPos"] = OSD_showPos;
  save["OSD_showVelocity"] = OSD_showVelocity;
  save["OSD_showMoveState"] = OSD_showMoveState;

  save["menuX"] = menuX;
  save["menuY"] = menuY;

  save["hide_reticle"] = hide_reticle;
  return save;
}

constexpr float DEBOUNCE_TIME = 2.0f;

void PatchConfig::RequestConfigSave() {
//  Logger::log("Requesting config save");
  requestConfigSave = true;
  requestSaveDebounce = ImGui::GetTime();
}

bool PatchConfig::ShouldSave() {
//  if (requestConfigSave) {
//    Logger::log("ShouldSave: %d, %f, %f\n", requestConfigSave, ImGui::GetTime(), requestSaveDebounce);
//  }
  return requestConfigSave && ImGui::GetTime() > requestSaveDebounce + 2.0f;
}
