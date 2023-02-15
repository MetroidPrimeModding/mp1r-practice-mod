#pragma once

#include "types.h"
#include "prime/CPlayerStateMP1.hpp"
#include "prime/CGameState.hpp"

class CStateManager {
public:
  CGameState *GameState();

  inline CStateManagerGameLogicMP1* GameLogic() {
      return reinterpret_cast<CStateManagerGameLogicMP1*>(reinterpret_cast<size_t>(this) + 0x4e0150);
  }
};
namespace GUI {
  void drawInventoryMenu();
}