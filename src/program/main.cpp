#include "lib.hpp"
#include "imgui_backend/imgui_impl_nvn.hpp"
#include "patches.hpp"
#include "logger/Logger.hpp"
#include "nn/fs.h"

#include "imgui_backend/imgui_nvn.h"
#include "helpers/InputHelper.h"

#include "ExceptionHandler.h"

#include "CGame/CGameState.h"

#define IMGUI_ENABLED true

#define BITFIELD_CHECKBOX(label, bf, ...)                                                                              \
  {                                                                                                                    \
    bool b = (bf);                                                                                                     \
    if (ImGui::Checkbox(label, &b)) {                                                                                  \
      (bf) = b;                                                                                                        \
      __VA_ARGS__                                                                                                      \
    }                                                                                                                  \
  }


void drawDebugWindow() {

  ImGui::Begin("Game Debug Window");
  ImGui::SetWindowSize(ImVec2(200, 100), ImGuiCond_FirstUseEver);

  if (ImGui::CollapsingHeader("General Game Toggles")) {
    BITFIELD_CHECKBOX("Toggle MP1 dash", PATCH_CONFIG.dash_enabled);
    ImGui::Checkbox("Toggle Skippable Cutscene Override", &CGameState::mCinematicForceSkippableOverride);
  }

  ImGui::End();
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

}

extern "C" NORETURN void exl_exception_entry() {
  /* TODO: exception handling */
  EXL_ABORT(0x420);
}