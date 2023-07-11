#include "PhotoModeMenu.hpp"
#include "patches.hpp"
#include "imgui.h"

#define REQ_CONFIG_IF(cond, ...)                                                                                       \
  {                                                                                                                    \
    if (cond) {                                                                                                        \
      PATCH_CONFIG.RequestConfigSave();                                                                                \
      __VA_ARGS__                                                                                                      \
    }                                                                                                                  \
  }

namespace GUI {
  bool is_time_stopped = false;

  const bool shouldHideAll() {
      return PATCH_CONFIG.hide_hud_when_time_stopped && is_time_stopped;
  }

  void drawPhotoModeMenu() {
    if (ImGui::TreeNode("Photo Mode")) {
      REQ_CONFIG_IF(ImGui::Checkbox("Stop time (left stick + L)", &PATCH_CONFIG.enable_stop_time));
      REQ_CONFIG_IF(ImGui::Checkbox("Hide arm cannon", &PATCH_CONFIG.hide_cannon));
      REQ_CONFIG_IF(ImGui::Checkbox("Hide HUD", &PATCH_CONFIG.hide_hud));
      REQ_CONFIG_IF(ImGui::Checkbox("Auto-hide HUD & cannon when time stopped", &PATCH_CONFIG.hide_hud_when_time_stopped));

      ImGui::TreePop();
    }
  }
}