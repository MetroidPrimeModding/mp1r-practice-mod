#pragma once

#include "lib.hpp"

struct PatchConfig {
  bool dash_enabled: 1 = true;
};

extern PatchConfig PATCH_CONFIG;

void runCodePatches();