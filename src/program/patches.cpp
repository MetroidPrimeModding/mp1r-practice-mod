#include "patches.hpp"
#include "logger/Logger.hpp"
#include "prime/CObjectId.hpp"
#include "prime/CPlayerStateMP1.hpp"
#include "prime/CGameState.hpp"
#include "prime/CPlayerMP1.hpp"
#include "prime/CFinalInput.hpp"
#include "prime/CSamusHudMP1.hpp"
#include "PlayerMenu.hpp"
#include "PhotoModeMenu.hpp"
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

    if (GUI::desiredTime > 0 && GUI::hasDesiredTimeData) {
      GUI::hasDesiredTimeData = false;
      if (PATCH_CONFIG.load_time_with_pos || PATCH_CONFIG.load_time_separately) {
        gameState->GetPlayTime() = GUI::desiredTime;
      }
      GUI::desiredTime = -1; //safety so we don't double-load
    }

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
        if (input.GetDigitalHeld(EControl::RIGHT_STICK_CLICK) && input.GetDigitalHeld(EControl::DPAD_LEFT)) {
          GUI::loadTime();
        }
        if (input.GetDigitalHeld(EControl::RIGHT_STICK_CLICK) && input.GetDigitalHeld(EControl::DPAD_RIGHT)) {
          GUI::saveTime();
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

    if (!InputHelper::isInputToggled() && GUI::is_time_stopped) {
      auto rot = CRelAngle(InputHelper::getRightStickX() * -0.05f);
      CTransform4f rotT = CTransform4f::Identity();
      rotT.RotateLocalY(rot);
      GUI::lastKnownTransform = GUI::lastKnownTransform * rotT;
      thiz->SetTransform(GUI::lastKnownTransform);

      const float maxPitch = gpTweakPlayer->GetMaxFreeLookPitchAngle();
      float rotPitch = InputHelper::getRightStickY() * 0.05f;
      float currPitch = *thiz->GetFreeLookPitchAngle();
      *thiz->GetFreeLookPitchAngle() = std::clamp(currPitch + rotPitch, -maxPitch, maxPitch);

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
    if (!PATCH_CONFIG.hide_reticle && !GUI::shouldHideAll()) {
      Orig(self, mgr);
    }
  }
};

HOOK_DEFINE_TRAMPOLINE(CStateManagerGameLogicMP1_GameMainLoop){
  static void Callback(CStateManagerGameLogicMP1 *self, CStateManager &mgr, CStateManagerUpdateAccess &mgrUpdAcc, float dt) {
    if(PATCH_CONFIG.enable_stop_time) {
      if (InputHelper::isHoldLeftStick() && InputHelper::isPressL() || InputHelper::isPressLeftStick() && InputHelper::isHoldL()) {
        const auto new_state = GUI::is_time_stopped ? CStateManagerGameLogicMP1::EGameState::Running
                                                    : CStateManagerGameLogicMP1::EGameState::SoftPaused; 
        self->SetGameState(new_state);
        GUI::is_time_stopped = !GUI::is_time_stopped;
        if (GUI::is_time_stopped) {
            self->PlayerActor()->BreakOrbit((CPlayerMP1::EOrbitBrokenType)1, mgr);
        }
      }
    }
    
    Orig(self, mgr, mgrUpdAcc, dt);
  }
};

HOOK_DEFINE_TRAMPOLINE(CPlayerMP1_RenderGun) {
  static void Callback(const CPlayerMP1* self, const CStateManager& mgr, const CVector3f& pos) {
    if (!PATCH_CONFIG.hide_cannon && !GUI::shouldHideAll()){
        Orig(self, mgr, pos);
    }
  }
};

HOOK_DEFINE_TRAMPOLINE(CSamusHudMP1_Draw){static void Callback(
    const CSamusHudMP1* self, const CStateManager& stateManager, float alpha, uint32_t param_3,
        const rstl::enum_bit_field<CSamusHudMP1::EHudDrawFilter, unsigned int, (CSamusHudMP1::EHudDrawFilter)3>& hudDrawFilterFlags) {
    if (!PATCH_CONFIG.hide_hud && !GUI::shouldHideAll()) {
        Orig(self, stateManager, alpha, param_3, hudDrawFilterFlags);
    }
  }
};

HOOK_DEFINE_TRAMPOLINE(CSamusHudMP1_DrawHelmet){
  static void Callback(const CSamusHudMP1* self, const CStateManager& mgr, float camYOffset) {
    if (!PATCH_CONFIG.hide_hud && !GUI::shouldHideAll()) {
        Orig(self, mgr, camYOffset);
    }
  }
};

HOOK_DEFINE_TRAMPOLINE(CAutoMapperMP1_Update){
  static void Callback(CAutoMapperMP1* self, float f1, const CStateManager &csm, const CGuiCamera& cgc, float f2, bool b1) {
    if (PATCH_CONFIG.hide_hud || GUI::shouldHideAll()) {
      Orig(self, f1, csm, cgc, 0.f, b1);
    } else {
      Orig(self, f1, csm, cgc, f2, b1);
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
  CStateManagerGameLogicMP1_GameMainLoop::InstallAtFuncPtr(&CStateManagerGameLogicMP1::GameMainLoop);
  CPlayerMP1_RenderGun::InstallAtFuncPtr(&CPlayerMP1::RenderGun);
  CSamusHudMP1_Draw::InstallAtFuncPtr(&CSamusHudMP1::Draw);
  CSamusHudMP1_DrawHelmet::InstallAtFuncPtr(&CSamusHudMP1::DrawHelmet);
  CAutoMapperMP1_Update::InstallAtFuncPtr(&CAutoMapperMP1::Update);
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
  dash_enabled = json.value("dash_enabled", dash_enabled);
  pos_edit = json.value("pos_edit", pos_edit);
  load_time_with_pos = json.value("load_time_with_pos", load_time_with_pos);
  load_time_separately = json.value("load_time_separately", load_time_separately);

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
  enable_stop_time = json.value("enable_stop_time", enable_stop_time);
  hide_cannon = json.value("hide_cannon", hide_cannon);
  hide_hud = json.value("hide_hud", hide_hud);
  hide_hud_when_time_stopped = json.value("hide_hud_when_time_stopped", hide_hud_when_time_stopped);
}


json PatchConfig::createSaveJson() {
  json save;
  save["dash_enabled"] = dash_enabled;
  save["pos_edit"] = pos_edit;
  save["load_time_with_pos"] = load_time_with_pos;
  save["load_time_separately"] = load_time_separately;

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
  save["enable_stop_time"] = enable_stop_time;
  save["hide_cannon"] = hide_cannon;
  save["hide_hud"] = hide_hud;
  save["hide_hud_when_time_stopped"] = hide_hud_when_time_stopped;
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
