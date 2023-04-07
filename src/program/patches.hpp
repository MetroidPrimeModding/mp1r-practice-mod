#pragma once

#include "lib.hpp"
#include "nlohmann/json_fwd.hpp"

struct PatchConfig {
  // NOT SAVED
  bool invulnerable = false;

  bool dash_enabled = false;
  bool pos_edit = true;
  bool load_time = false;

  bool OSD_showInput = true;
  float OSD_inputScale = 1.5f;

  bool OSD_showMonitor = true;
  bool OSD_showIGT = true;
  bool OSD_showPos = true;
  bool OSD_showVelocity = true;
  bool OSD_showMoveState = false;

  float menuX = 10.f;
  float menuY = 150.f;

  bool hide_reticle = false;

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

