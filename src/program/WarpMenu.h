#pragma once

#include "types.h"
#include "prime/CGameState.hpp"

namespace GUI {
  void warpToRoom(const CObjectId& worldId, const CObjectId& areaId);

  void drawWarpMenu();
}
