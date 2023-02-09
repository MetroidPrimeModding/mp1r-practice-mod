#include "patches.hpp"
#include "logger/Logger.hpp"
#include "CObject/CObjectId.h"

namespace patch = exl::patch;
namespace inst = exl::armv8::inst;
namespace reg = exl::armv8::reg;

PatchConfig PATCH_CONFIG{};

HOOK_DEFINE_INLINE(Dash_ScanVisorCheck) {
  static void Callback(exl::hook::InlineCtx *ctx) {
    if (PATCH_CONFIG.dash_enabled) {
      ctx->W[8] = 0;
    }
  }
};

HOOK_DEFINE_INLINE(Dash_Speed) {
  static void Callback(exl::hook::InlineCtx *ctx) {
    if (PATCH_CONFIG.dash_enabled) {
      ctx->W[8] = 0;
    }
  }
};

HOOK_DEFINE_INLINE(Dash_VelocityClamp) {
  static void Callback(exl::hook::InlineCtx *ctx) {
    if (PATCH_CONFIG.dash_enabled) {
      ctx->W[8] = 0;
    }
  }
};

void runCodePatches() {
  Dash_ScanVisorCheck::InstallAtOffset(0xceeab0ull);
  Dash_ScanVisorCheck::InstallAtOffset(0xceeac4ull);
  Dash_Speed::InstallAtOffset(0xcef7a8ull);
  Dash_VelocityClamp::InstallAtOffset(0xcee31Cull);
  Dash_VelocityClamp::InstallAtOffset(0xcee334ull);
}