#pragma once

class RProductionFlags {
public:
  static bool gIsDemoVersion;
  static bool gUseProductionFrontEnd;
  static int gProductionFrontEndId;
  static bool gLogGameStartup;
  static float gDemoVersion_InLevelResetTimeInSeconds;
  static float gDemoVersion_InGameResetTimeInSeconds;
  static bool gCinemaSkipIgnorePreviouslySeenFlag;
  static bool gMarketingBuild;
  static bool gDisableSimplePerformanceMetrics;
  static bool gProductionMetricsMode;
  static bool gForceWarnMetricsInactive;
  static bool gDisableConsolePrintf;
  static bool gAllowInfiniteLoopFinder;
  static bool gResetGameOnQuit;
  static bool gEnableWarnMetrics;
  static bool gEnableCinematicDebug;
  static bool gIsEPDVersionForScripting;
  static bool gEnableAllowNonProductionTokenReload;
  static bool gIsTesterVersionForScripting;
  static bool gEnableCodeAndDataVersionDisplay;
  static bool gEnableNetworkClientCreation;
  static bool gEnableProductionDumpExceptionHandler;
  static bool gEnableProductionLoadTimeCheats;
  static bool gEnableNonProductionControllerConnectionLimit;
  static bool gEnableNonProductionControllerSupportAppletOnInsufficientConnectionCountE;

};