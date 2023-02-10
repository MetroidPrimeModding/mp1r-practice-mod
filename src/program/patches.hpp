#pragma once

#include "lib.hpp"

struct PatchConfig {
  bool dash_enabled: 1 = false;
};

extern PatchConfig PATCH_CONFIG;

class CStateManager;
extern CStateManager* mostRecentStateManager;

void runCodePatches();