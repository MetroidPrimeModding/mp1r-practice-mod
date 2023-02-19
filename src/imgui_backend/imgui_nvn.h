#pragma once

#include "types.h"

#define IMGUINVN_ADDDRAW(FuncBody) nvnImGui::addDrawFunc([]() {             \
FuncBody                                                                    \
});                                                                         \

namespace nvnImGui {

  typedef void (*ProcDrawFunc)();
  typedef void (*InitFunc)();

  void InstallHooks();

  bool InitImGui();

  void procDraw();

  void addDrawFunc(ProcDrawFunc func);
  void addInitFunc(InitFunc func);
}
