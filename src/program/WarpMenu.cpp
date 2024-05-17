#include <helpers/InputHelper.h>
#include <imgui.h>
#include <logger/Logger.hpp>
#include <program/WarpMenu.h>
#include "patches.hpp"

namespace {
  struct RegionEntry {
    const char* mEnglishName = nullptr;
    CObjectId mAreaId;
  };

  template<unsigned int N>
  bool drawAreaEntries(const char* englishName, CObjectId& regionId, const RegionEntry(&entries)[N]) {
    bool warped = false;
    if(ImGui::BeginMenu(englishName)) {
      ImGui::MenuItem("Warp to room", NULL, false, false);
      for (auto& entry : entries) {
        if(ImGui::MenuItem(entry.mEnglishName)) {
          GUI::warpToRoom(regionId, entry.mAreaId);
          warped = true;
        }
      }
      ImGui::EndMenu();
    }
    return warped;
  }
}

#include "RegionEntries.h"

namespace GUI {

  CObjectId sOverMasterId = CObjectId("73f2640c-cfb2-407b-a786-47a263ef0001"); // Tallon Overworld
  CObjectId sRuinsWorldId = CObjectId("914061a2-c17a-4826-94f5-be4faf90cae7"); // Chozo Ruins
  CObjectId sLavaMasterId = CObjectId("f5712eed-dde1-47bf-8670-d0664954861a"); // Magmoor Caverns
  CObjectId sIceWorldId = CObjectId("81b62adb-88f3-46f0-a43c-b8ccdfc354df"); // Phendrana Drifts
  CObjectId sMinesMasterId = CObjectId("d3e02263-def7-4a66-bef4-26a45e8c59a8"); // Phazon Mines
  CObjectId sCraterMasterId = CObjectId("73318837-09ac-4dbb-bc66-d26950259b27"); // Impact Crater
  CObjectId sIntroMasterId = CObjectId("4b5e8635-0f43-4ffb-9427-2e62186e517d"); // Space Pirate Frigate

  void drawWarpMenu() {
    bool warped = false;
    if (ImGui::TreeNode("Warp")) {
      warped = drawAreaEntries("Tallon Overworld", sOverMasterId, sOverMasterEntries);
      warped = warped || drawAreaEntries("Chozo Ruins", sRuinsWorldId, sRuinsWorldEntries);
      warped = warped || drawAreaEntries("Magmoor Caverns", sLavaMasterId, sLavaMasterEntries);
      warped = warped || drawAreaEntries("Phendrana Drifts", sIceWorldId, sIceWorldEntries);
      warped = warped || drawAreaEntries("Phazon Mines", sMinesMasterId, sMinesMasterEntries);
      warped = warped || drawAreaEntries("Impact Crater", sCraterMasterId, sCraterMasterEntries);
      warped = warped || drawAreaEntries("Space Pirate Frigate", sIntroMasterId, sIntroMasterEntries);
      ImGui::TreePop();
    }

    if(warped) {
      ImGui::SetWindowCollapsed(true);
    }
  }

  void warpToRoom(const CObjectId& worldId, const CObjectId& areaId) {
    if(mostRecentStateManager == nullptr) {
      Logger::log("State Manager is not Set! Cannot Warp.\n");
      return;
    }

    gpGameState->SetCurrentWorldId(worldId);
    gpGameState->CurrentWorldState()->SetDesiredAreaAssetId(areaId);

    CGameState::ContinueWithNewInitialState(mostRecentStateManager->GameState(), CGameState::SInitialState(areaId));

    mostRecentStateManager->GameInstanceState().QuitGame(NGameInstanceState::kQR_Warp);
  }
}