#include <cmath>
#include "lib.hpp"
#include "imgui_backend/imgui_impl_nvn.hpp"
#include "patches.hpp"
#include "logger/Logger.hpp"
#include "nn/fs.h"

#include "imgui_backend/imgui_nvn.h"
#include "helpers/InputHelper.h"

#include "ExceptionHandler.h"

#include "prime/CGameState.hpp"
#include "InventoryMenu.hpp"
#include "PlayerMenu.hpp"
#include "ProductionFlagMenu.hpp"
#include "InputWindow.hpp"

#define IMGUI_ENABLED true

#define BITFIELD_CHECKBOX(label, bf, ...)                                                                              \
  {                                                                                                                    \
    bool b = (bf);                                                                                                     \
    if (ImGui::Checkbox(label, &b)) {                                                                                  \
      (bf) = b;                                                                                                        \
      __VA_ARGS__                                                                                                      \
    }                                                                                                                  \
  }

#define REQ_CONFIG_IF(cond, ...) \
  {                           \
    if (cond) {               \
       PATCH_CONFIG.RequestConfigSave(); \
      __VA_ARGS__             \
    }                         \
  }

bool wasJustOpened = false;

void drawDebugWindow() {
  ImGui::SetNextWindowSizeConstraints(ImVec2(200, 100), ImVec2(500, 500));
  ImGui::SetNextWindowCollapsed(!InputHelper::isInputToggled());
  if (InputHelper::isInputToggled()) {
    if (!wasJustOpened) {
      wasJustOpened = true;
      ImGui::SetNextWindowPos(ImVec2(PATCH_CONFIG.menuX, PATCH_CONFIG.menuY), ImGuiCond_None);
      ImGui::SetNextWindowFocus();
    }
  } else {
    ImGui::SetNextWindowPos(ImVec2(-1600, 10), ImGuiCond_None);
    wasJustOpened = false;
  }
  if (ImGui::Begin("Practice Mod", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    auto pos = ImGui::GetWindowPos();
    if (pos.x >= 0) {
      float dX = fabs(PATCH_CONFIG.menuX - pos.x);
      float dY = fabs(PATCH_CONFIG.menuY - pos.y);

      PATCH_CONFIG.menuX = pos.x;
      PATCH_CONFIG.menuY = pos.y;
      if (dX > 0.01f || dY > 0.01f) {
        PATCH_CONFIG.RequestConfigSave();
      }
    }
//  ImGui::SetWindowSize(ImVec2(200, 100), ImGuiCond_FirstUseEver);

    if (ImGui::TreeNode("Settings")) {
      REQ_CONFIG_IF(ImGui::Checkbox("Show input", &PATCH_CONFIG.OSD_showInput));
      REQ_CONFIG_IF(ImGui::SliderFloat("Input scale", &PATCH_CONFIG.OSD_inputScale, 0.5f, 10.f, "%.1f"));

      REQ_CONFIG_IF(ImGui::Checkbox("Show monitor window", &PATCH_CONFIG.OSD_showMonitor));
      REQ_CONFIG_IF(ImGui::Checkbox("Show IGT", &PATCH_CONFIG.OSD_showIGT));
      REQ_CONFIG_IF(ImGui::Checkbox("Show position", &PATCH_CONFIG.OSD_showPos));
      REQ_CONFIG_IF(ImGui::Checkbox("Show velocity", &PATCH_CONFIG.OSD_showVelocity));
      REQ_CONFIG_IF(ImGui::Checkbox("Show movement state", &PATCH_CONFIG.OSD_showMoveState));

      REQ_CONFIG_IF(ImGui::Checkbox("Allow skipping all cutscenes", &CGameState::mCinematicForceSkippableOverride));

      ImGui::TreePop();
    }

    GUI::drawInventoryMenu();
    GUI::drawPlayerMenu();
//  GUI::drawProductionFlagMenu();

  }
  InputHelper::toggleInput = !ImGui::IsWindowCollapsed();
  ImGui::End();

  GUI::drawInputWindow();
  GUI::drawMonitorWindow();

  if (PATCH_CONFIG.ShouldSave()) {
    PATCH_CONFIG.saveConfig();
  }
}

void init() {
  PATCH_CONFIG.loadConfig();
}

extern "C" void exl_main(void *x0, void *x1) {
  /* Setup hooking enviroment. */
  exl::hook::Initialize();

  nn::os::SetUserExceptionHandler(exception_handler, nullptr, 0, nullptr);

  R_ABORT_UNLESS(Logger::instance().init(LOGGER_IP, 3080).value);

  Logger::log("Hello, world\n");

  runCodePatches();

  // ImGui Hooks
  nvnImGui::addInitFunc(init);
  nvnImGui::InstallHooks();
  nvnImGui::addDrawFunc(drawDebugWindow);

  CGameState::mCinematicForceSkippableOverride = true;
}

extern "C" NORETURN void exl_exception_entry() {
  /* TODO: exception handling */
  EXL_ABORT(0x420);
}