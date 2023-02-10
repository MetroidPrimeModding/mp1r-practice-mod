#include "ProductionFlagMenu.hpp"
#include "imgui.h"
#include "prime/RProductionFlags.hpp"

namespace GUI {
  void drawProductionFlagMenu() {
    if (ImGui::TreeNode("Production Flags")) {
      ImGui::Text("Be careful! These can be dangerous!");
      ImGui::Checkbox("gIsDemoVersion", &RProductionFlags::gIsDemoVersion);
      ImGui::Checkbox("gUseProductionFrontEnd", &RProductionFlags::gUseProductionFrontEnd);
      ImGui::InputInt("gProductionFrontEndId", &RProductionFlags::gProductionFrontEndId);
      ImGui::Checkbox("gLogGameStartup", &RProductionFlags::gLogGameStartup);
      ImGui::InputFloat("gDemoVersion_InLevelResetTimeInSeconds",
                        &RProductionFlags::gDemoVersion_InLevelResetTimeInSeconds);
      ImGui::InputFloat("gDemoVersion_InGameResetTimeInSeconds",
                        &RProductionFlags::gDemoVersion_InGameResetTimeInSeconds);
      ImGui::Checkbox("gCinemaSkipIgnorePreviouslySeenFlag", &RProductionFlags::gCinemaSkipIgnorePreviouslySeenFlag);
      ImGui::Checkbox("gMarketingBuild", &RProductionFlags::gMarketingBuild);
      ImGui::Checkbox("gDisableSimplePerformanceMetrics", &RProductionFlags::gDisableSimplePerformanceMetrics);
      ImGui::Checkbox("gProductionMetricsMode", &RProductionFlags::gProductionMetricsMode);
      ImGui::Checkbox("gForceWarnMetricsInactive", &RProductionFlags::gForceWarnMetricsInactive);
      ImGui::Checkbox("gDisableConsolePrintf", &RProductionFlags::gDisableConsolePrintf);
      ImGui::Checkbox("gAllowInfiniteLoopFinder", &RProductionFlags::gAllowInfiniteLoopFinder);
      ImGui::Checkbox("gResetGameOnQuit", &RProductionFlags::gResetGameOnQuit);
      ImGui::Checkbox("gEnableWarnMetrics", &RProductionFlags::gEnableWarnMetrics);
      ImGui::Checkbox("gEnableCinematicDebug", &RProductionFlags::gEnableCinematicDebug);
      ImGui::Checkbox("gIsEPDVersionForScripting", &RProductionFlags::gIsEPDVersionForScripting);
      ImGui::Checkbox("gEnableAllowNonProductionTokenReload", &RProductionFlags::gEnableAllowNonProductionTokenReload);
      ImGui::Checkbox("gIsTesterVersionForScripting", &RProductionFlags::gIsTesterVersionForScripting);
      ImGui::Checkbox("gEnableCodeAndDataVersionDisplay", &RProductionFlags::gEnableCodeAndDataVersionDisplay);
      ImGui::Checkbox("gEnableNetworkClientCreation", &RProductionFlags::gEnableNetworkClientCreation);
      ImGui::Checkbox("gEnableProductionDumpExceptionHandler",
                      &RProductionFlags::gEnableProductionDumpExceptionHandler);
      ImGui::Checkbox("gEnableProductionLoadTimeCheats", &RProductionFlags::gEnableProductionLoadTimeCheats);
      ImGui::Checkbox("gEnableNonProductionControllerConnectionLimit",
                      &RProductionFlags::gEnableNonProductionControllerConnectionLimit);
//      ImGui::Checkbox("gEnableNonProductionControllerSupportAppletOnInsufficientConnectionCountE",
//                      &RProductionFlags::gEnableNonProductionControllerSupportAppletOnInsufficientConnectionCountE);

      ImGui::TreePop();
    }
  }
}