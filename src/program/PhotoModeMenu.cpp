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
  const float defaultFirstPersonFov = CCameraManagerMP1::GetDefaultFirstPersonVerticalFOV();
  float verticalFov = defaultFirstPersonFov;
  bool shouldUpdateVerticalFov = false;
  float viewRoll = 0.f;
  CGameCameraMP1* fpCamera = nullptr;

  const bool shouldHideAll() {
      return PATCH_CONFIG.hide_hud_when_time_stopped && is_time_stopped;
  }

  void drawPhotoModeMenu() {
    if (ImGui::TreeNode("Photo Mode")) {
      REQ_CONFIG_IF(ImGui::Checkbox("Stop time (left stick + L)", &PATCH_CONFIG.enable_stop_time));
      REQ_CONFIG_IF(ImGui::Checkbox("Hide arm cannon", &PATCH_CONFIG.hide_cannon));
      REQ_CONFIG_IF(ImGui::Checkbox("Hide HUD", &PATCH_CONFIG.hide_hud));
      REQ_CONFIG_IF(ImGui::Checkbox("Auto-hide HUD & cannon when time stopped", &PATCH_CONFIG.hide_hud_when_time_stopped));

      // NOT SAVED
      // Vertical FOV
      ImGui::PushID("VerticalFOV");
      if (ImGui::DragFloat("", &verticalFov, 1.0, 1.0, 170.f, "%.2f")) {
        shouldUpdateVerticalFov = true;
      }
      ImGui::SameLine();
      if (ImGui::Button("Reset")) {
        verticalFov = defaultFirstPersonFov;
        shouldUpdateVerticalFov = true;
      }
      ImGui::SameLine();
      ImGui::Text("Vertical FOV");
      ImGui::PopID();

      // View Roll
      ImGui::PushID("ViewRoll");
      ImGui::DragFloat("", &viewRoll, 1.0, -90.f, 90.f, "%.2f");
      ImGui::SameLine();
      if (ImGui::Button("Reset")) {
        viewRoll = 0.f;
      }
      ImGui::SameLine();
      ImGui::Text("View Roll");
      ImGui::PopID();

      ImGui::TreePop();
    }
  }
}