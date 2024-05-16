#pragma once

#include "lib.hpp"
#include "nlohmann/json_fwd.hpp"

struct PatchConfig {
  // NOT SAVED
  bool invulnerable = false;

  bool dash_enabled = false;
  bool pos_edit = true;
  bool load_time_with_pos = false;
  bool load_time_separately = false;

  bool OSD_showInput = true;
  float OSD_inputScale = 2.5f;

  bool OSD_showMonitor = true;
  bool OSD_showIGT = true;
  bool OSD_showPos = true;
  bool OSD_showVelocity = true;
  bool OSD_showMoveState = false;

  float menuX = 10.f;
  float menuY = 150.f;

  bool hide_reticle = false;

  bool enable_stop_time = false;
  bool hide_cannon = false;
  bool hide_hud = false;
  bool hide_hud_when_time_stopped = true;

  void RequestConfigSave();
  bool ShouldSave();

  void loadConfig();
  void saveConfig();

private:
  void loadFromJson(const nlohmann::json &json);
  nlohmann::json createSaveJson();
  bool requestConfigSave;
  double requestSaveDebounce;
};

extern PatchConfig PATCH_CONFIG;

class CStateManager;
extern CStateManager* mostRecentStateManager;

void runCodePatches();

