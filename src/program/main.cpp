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

ImVec2 lastWindowPos = ImVec2(160, 10);
bool wasJustOpened = false;

void drawDebugWindow() {
  ImGui::SetNextWindowSizeConstraints(ImVec2(200, 100), ImVec2(500, 500));
  ImGui::SetNextWindowCollapsed(!InputHelper::isInputToggled());
  if (InputHelper::isInputToggled()) {
    if (!wasJustOpened) {
      wasJustOpened = true;
      ImGui::SetNextWindowPos(lastWindowPos, ImGuiCond_None);
    }
  } else {
    ImGui::SetNextWindowPos(ImVec2(-1600, 10), ImGuiCond_None);
    wasJustOpened = false;
  }
  if (ImGui::Begin("Practice Mod", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
    auto pos = ImGui::GetWindowPos();
    if (pos.x >= 0) {
      lastWindowPos = pos;
    }
//  ImGui::SetWindowSize(ImVec2(200, 100), ImGuiCond_FirstUseEver);

    if (ImGui::TreeNode("General Game Toggles")) {
//      BITFIELD_CHECKBOX("Toggle MP1 dash", PATCH_CONFIG.dash_enabled);
      ImGui::Checkbox("Toggle Skippable Cutscene Override", &CGameState::mCinematicForceSkippableOverride);
      BITFIELD_CHECKBOX("Show input", PATCH_CONFIG.show_input);
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
}

extern "C" void exl_main(void *x0, void *x1) {
  /* Setup hooking enviroment. */
  exl::hook::Initialize();

  nn::os::SetUserExceptionHandler(exception_handler, nullptr, 0, nullptr);

  R_ABORT_UNLESS(Logger::instance().init(LOGGER_IP, 3080).value);

  Logger::log("Hello, world\n");

  runCodePatches();

  // ImGui Hooks
#if IMGUI_ENABLED
  nvnImGui::InstallHooks();

  nvnImGui::addDrawFunc(drawDebugWindow);
#endif

  CGameState::mCinematicForceSkippableOverride = true;
}

extern "C" NORETURN void exl_exception_entry() {
  /* TODO: exception handling */
  EXL_ABORT(0x420);
}