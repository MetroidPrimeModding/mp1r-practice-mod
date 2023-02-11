#pragma once

#include "lib.hpp"

struct PatchConfig {
  bool dash_enabled: 1 = false;
  bool show_input: 1 = true;
  bool pos_edit: 1 = true;
};

extern PatchConfig PATCH_CONFIG;

class CStateManager;
extern CStateManager* mostRecentStateManager;

void runCodePatches();