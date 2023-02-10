//
// Created by pwootage on 2/9/23.
//

#include "InventoryMenu.hpp"
#include "imgui.h"
#include "logger/Logger.hpp"

static constexpr CPlayerStateMP1::EItemType GeneralItems[] = {
    CPlayerStateMP1::EItemType::EnergyTanks, CPlayerStateMP1::EItemType::CombatVisor, CPlayerStateMP1::EItemType::ScanVisor,
    CPlayerStateMP1::EItemType::ThermalVisor, CPlayerStateMP1::EItemType::XRayVisor, CPlayerStateMP1::EItemType::GrappleBeam,
    CPlayerStateMP1::EItemType::SpaceJumpBoots, CPlayerStateMP1::EItemType::PowerSuit, CPlayerStateMP1::EItemType::VariaSuit,
    CPlayerStateMP1::EItemType::GravitySuit, CPlayerStateMP1::EItemType::PhazonSuit,
};

static constexpr CPlayerStateMP1::EItemType WeaponItems[] = {
    CPlayerStateMP1::EItemType::Missiles, CPlayerStateMP1::EItemType::PowerBeam, CPlayerStateMP1::EItemType::IceBeam,
    CPlayerStateMP1::EItemType::WaveBeam, CPlayerStateMP1::EItemType::PlasmaBeam, CPlayerStateMP1::EItemType::SuperMissile,
    CPlayerStateMP1::EItemType::Flamethrower, CPlayerStateMP1::EItemType::IceSpreader, CPlayerStateMP1::EItemType::Wavebuster,
    CPlayerStateMP1::EItemType::ChargeBeam,
};

static constexpr CPlayerStateMP1::EItemType MorphBallItems[] = {
    CPlayerStateMP1::EItemType::PowerBombs, CPlayerStateMP1::EItemType::MorphBall, CPlayerStateMP1::EItemType::MorphBallBombs,
    CPlayerStateMP1::EItemType::BoostBall, CPlayerStateMP1::EItemType::SpiderBall,
};

static constexpr CPlayerStateMP1::EItemType ArtifactItems[] = {
    CPlayerStateMP1::EItemType::Truth, CPlayerStateMP1::EItemType::Strength, CPlayerStateMP1::EItemType::Elder,
    CPlayerStateMP1::EItemType::Wild, CPlayerStateMP1::EItemType::Lifegiver, CPlayerStateMP1::EItemType::Warrior,
    CPlayerStateMP1::EItemType::Chozo, CPlayerStateMP1::EItemType::Nature, CPlayerStateMP1::EItemType::Sun,
    CPlayerStateMP1::EItemType::World, CPlayerStateMP1::EItemType::Spirit, CPlayerStateMP1::EItemType::Newborn,
};

struct SItemAmt {
  CPlayerStateMP1::EItemType item;
  int count{1};
};

static constexpr SItemAmt StartingItems[] = {
    {CPlayerStateMP1::EItemType::PowerSuit},
    {CPlayerStateMP1::EItemType::PowerBeam},
    {CPlayerStateMP1::EItemType::CombatVisor},
    {CPlayerStateMP1::EItemType::ScanVisor},
};

static constexpr SItemAmt LowPercentItems[] = {
    {CPlayerStateMP1::EItemType::EnergyTanks, 0},
    {CPlayerStateMP1::EItemType::Missiles, 5},

    {CPlayerStateMP1::EItemType::MorphBall},
    {CPlayerStateMP1::EItemType::MorphBallBombs},
    {CPlayerStateMP1::EItemType::PowerBombs, 4},

    {CPlayerStateMP1::EItemType::VariaSuit},
    {CPlayerStateMP1::EItemType::PhazonSuit},

    {CPlayerStateMP1::EItemType::WaveBeam},
    {CPlayerStateMP1::EItemType::IceBeam},
    {CPlayerStateMP1::EItemType::PlasmaBeam},

    {CPlayerStateMP1::EItemType::XRayVisor},
};

static constexpr SItemAmt AnyPercentItems[] = {
    {CPlayerStateMP1::EItemType::EnergyTanks, 3},
    {CPlayerStateMP1::EItemType::Missiles, 25},

    {CPlayerStateMP1::EItemType::MorphBall},
    {CPlayerStateMP1::EItemType::MorphBallBombs},
    {CPlayerStateMP1::EItemType::BoostBall},
    {CPlayerStateMP1::EItemType::PowerBombs, 5},

    {CPlayerStateMP1::EItemType::VariaSuit},
    {CPlayerStateMP1::EItemType::PhazonSuit},

    {CPlayerStateMP1::EItemType::WaveBeam},
    {CPlayerStateMP1::EItemType::IceBeam},
    {CPlayerStateMP1::EItemType::PlasmaBeam},
    {CPlayerStateMP1::EItemType::ChargeBeam},

    {CPlayerStateMP1::EItemType::XRayVisor},
    {CPlayerStateMP1::EItemType::ThermalVisor},

    {CPlayerStateMP1::EItemType::SpaceJumpBoots},
};

namespace GUI {
  void RenderItemType(CPlayerStateMP1 *playerState, CPlayerStateMP1::EItemType itemType);
  const char *ItemTypeToName(CPlayerStateMP1::EItemType type);
  static inline void RenderItemsDualColumn(CPlayerStateMP1 *playerState, const CPlayerStateMP1::EItemType *items, int start, int end);

  void clearItems(CPlayerStateMP1 *playerState);

  void refillItems(CPlayerStateMP1 *playerState);

  void drawInventoryMenu() {
    if (gpGameState == nullptr ) return;
    auto *playerState = CStateManagerGameLogicMP1::PlayerState();
    if (playerState == nullptr) return;

    if (ImGui::TreeNode("Inventory")) {
//      for (int i = 0; i < int(CPlayerStateMP1::EItemType::Max); i++) {
//        auto itemType = static_cast<CPlayerStateMP1::EItemType>(i);
//        RenderItemType(playerState, itemType);
//      }

      if (ImGui::Button("Refill")) {
        refillItems(playerState);
      }
//      {
//        ImGui::SameLine();
//        auto& mapWorldInfo = *g_GameState->CurrentWorldState().MapWorldInfo();
//        bool mapStationUsed = mapWorldInfo.GetMapStationUsed();
//        if (ImGui::Checkbox("Area map", &mapStationUsed)) {
//          mapWorldInfo.SetMapStationUsed(mapStationUsed);
//        }
//      }
      ImGui::SameLine();
      if (ImGui::Button("All")) {
        for (int i = 0; i < int(CPlayerStateMP1::EItemType::Max); i++) {
          auto itemType = static_cast<CPlayerStateMP1::EItemType>(i);
          int maxValue = CPlayerStateMP1::GetItemMax(itemType);
          playerState->SetPickup(itemType, maxValue);
          playerState->SetPowerUp(itemType, maxValue);
        }
      }
      ImGui::SameLine();
      if (ImGui::Button("None")) {
        clearItems(playerState);
      }
      ImGui::SameLine();
      if (ImGui::Button("Start")) {
        clearItems(playerState);
        for (auto v : StartingItems) playerState->SetPowerUp(v.item, v.count);
      }
      ImGui::SameLine();
      if (ImGui::Button("Any%")) {
        clearItems(playerState);
        for (auto v : StartingItems) playerState->SetPowerUp(v.item, v.count);
        for (auto v : AnyPercentItems) playerState->SetPowerUp(v.item, v.count);
        for (auto v : ArtifactItems) playerState->SetPowerUp(v, 1);
        refillItems(playerState);
      }
      ImGui::SameLine();
      if (ImGui::Button("21%")) {
        clearItems(playerState);
        for (auto v : StartingItems) playerState->SetPowerUp(v.item, v.count);
        for (auto v : LowPercentItems) playerState->SetPowerUp(v.item, v.count);
        for (auto v : ArtifactItems) playerState->SetPowerUp(v, 1);
        refillItems(playerState);
      }

      if (ImGui::BeginTabBar("Items")) {
        if (ImGui::BeginTabItem("General")) {
          RenderItemType(playerState, GeneralItems[0]); // full width
          RenderItemsDualColumn(playerState, GeneralItems, 1, sizeof(GeneralItems) / sizeof(GeneralItems[0]));
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Weapons")) {
          RenderItemType(playerState, WeaponItems[0]); // full width
          RenderItemsDualColumn(playerState, WeaponItems, 1, sizeof(WeaponItems) / sizeof(WeaponItems[0]));
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Morph Ball")) {
          RenderItemType(playerState, MorphBallItems[0]); // full width
          RenderItemsDualColumn(playerState, MorphBallItems, 1, sizeof(MorphBallItems) / sizeof(MorphBallItems[0]));
          ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Artifacts")) {
          ImGui::Text("NOTE: This doesn't affect Artifact Temple layers");
//          ImGui::Text("Use the Layers window to set them for progression");
          RenderItemsDualColumn(playerState, ArtifactItems, 0, sizeof(ArtifactItems) / sizeof(ArtifactItems[0]));
          ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
      }
      ImGui::TreePop();
    }
  }

  void refillItems(CPlayerStateMP1 *playerState) {
    for (int i = 0; i < int(CPlayerStateMP1::EItemType::Max); i++) {
      auto itemType = static_cast<CPlayerStateMP1::EItemType>(i);
      u32 maxValue = CPlayerStateMP1::GetItemMax(itemType);
      playerState->SetPickup(itemType, maxValue);
    }
  }

  void clearItems(CPlayerStateMP1 *playerState) {
    for (int i = 0; i < int(CPlayerStateMP1::EItemType::Max); i++) {
      auto itemType = static_cast<CPlayerStateMP1::EItemType>(i);
      playerState->SetPowerUp(itemType, 0);
    }
  }

  void RenderItemType(CPlayerStateMP1 *playerState, CPlayerStateMP1::EItemType itemType) {
    u32 maxValue = CPlayerStateMP1::GetItemMax(itemType);
    const char *name = ItemTypeToName(itemType);
    if (maxValue == 1) {
      bool enabled = playerState->GetItemCapacity(itemType) == 1;
      if (ImGui::Checkbox(name, &enabled)) {
        if (enabled) {
          Logger::log("Adding powerup: %d", int(itemType));
          playerState->AddPowerUp(itemType, 1);
          playerState->IncrPickUp(itemType, 1);
          playerState->SetPowerUp(itemType, 1);
          playerState->SetPickup(itemType, 1);
        } else {
          Logger::log("Removing powerup: %d", int(itemType));
          playerState->SetPowerUp(itemType, 0);
        }
        if (itemType == CPlayerStateMP1::EItemType::VariaSuit || itemType == CPlayerStateMP1::EItemType::PowerSuit ||
            itemType == CPlayerStateMP1::EItemType::GravitySuit || itemType == CPlayerStateMP1::EItemType::PhazonSuit) {
          // TODO
//          g_StateManager->Player()->AsyncLoadSuit(*g_StateManager);
        }
      }
    } else if (maxValue > 1) {
      int capacity = int(playerState->GetItemCapacity(itemType));
      float speed = 1;
      if (maxValue > 100) {
        speed = 5;
      }
      if (ImGui::DragInt(name, &capacity, speed, 0, int(maxValue), "%d", ImGuiSliderFlags_AlwaysClamp)) {
        playerState->SetPowerUp(itemType, capacity);
        playerState->SetPickup(itemType, capacity);
      }
    }
  }

  const char *ItemTypeToName(CPlayerStateMP1::EItemType type) {
    switch (type) {
      case CPlayerStateMP1::EItemType::PowerBeam:
        return "Power Beam";
      case CPlayerStateMP1::EItemType::IceBeam:
        return "Ice Beam";
      case CPlayerStateMP1::EItemType::WaveBeam:
        return "Wave Beam";
      case CPlayerStateMP1::EItemType::PlasmaBeam:
        return "Plasma Beam";
      case CPlayerStateMP1::EItemType::Missiles:
        return "Missiles";
      case CPlayerStateMP1::EItemType::ScanVisor:
        return "Scan Visor";
      case CPlayerStateMP1::EItemType::MorphBallBombs:
        return "Morph Ball Bombs";
      case CPlayerStateMP1::EItemType::PowerBombs:
        return "Power Bombs";
      case CPlayerStateMP1::EItemType::Flamethrower:
        return "Flamethrower";
      case CPlayerStateMP1::EItemType::ThermalVisor:
        return "Thermal Visor";
      case CPlayerStateMP1::EItemType::ChargeBeam:
        return "Charge Beam";
      case CPlayerStateMP1::EItemType::SuperMissile:
        return "Super Missile";
      case CPlayerStateMP1::EItemType::GrappleBeam:
        return "Grapple Beam";
      case CPlayerStateMP1::EItemType::XRayVisor:
        return "X-Ray Visor";
      case CPlayerStateMP1::EItemType::IceSpreader:
        return "Ice Spreader";
      case CPlayerStateMP1::EItemType::SpaceJumpBoots:
        return "Space Jump Boots";
      case CPlayerStateMP1::EItemType::MorphBall:
        return "Morph Ball";
      case CPlayerStateMP1::EItemType::CombatVisor:
        return "Combat Visor";
      case CPlayerStateMP1::EItemType::BoostBall:
        return "Boost Ball";
      case CPlayerStateMP1::EItemType::SpiderBall:
        return "Spider Ball";
      case CPlayerStateMP1::EItemType::PowerSuit:
        return "Power Suit";
      case CPlayerStateMP1::EItemType::GravitySuit:
        return "Gravity Suit";
      case CPlayerStateMP1::EItemType::VariaSuit:
        return "Varia Suit";
      case CPlayerStateMP1::EItemType::PhazonSuit:
        return "Phazon Suit";
      case CPlayerStateMP1::EItemType::EnergyTanks:
        return "Energy Tanks";
      case CPlayerStateMP1::EItemType::HealthRefill:
        return "Health Refill";
      case CPlayerStateMP1::EItemType::Wavebuster:
        return "Wavebuster";
      case CPlayerStateMP1::EItemType::Truth:
        return "Artifact of Truth";
      case CPlayerStateMP1::EItemType::Strength:
        return "Artifact of Strength";
      case CPlayerStateMP1::EItemType::Elder:
        return "Artifact of Elder";
      case CPlayerStateMP1::EItemType::Wild:
        return "Artifact of Wild";
      case CPlayerStateMP1::EItemType::Lifegiver:
        return "Artifact of Lifegiver";
      case CPlayerStateMP1::EItemType::Warrior:
        return "Artifact of Warrior";
      case CPlayerStateMP1::EItemType::Chozo:
        return "Artifact of Chozo";
      case CPlayerStateMP1::EItemType::Nature:
        return "Artifact of Nature";
      case CPlayerStateMP1::EItemType::Sun:
        return "Artifact of Sun";
      case CPlayerStateMP1::EItemType::World:
        return "Artifact of World";
      case CPlayerStateMP1::EItemType::Spirit:
        return "Artifact of Spirit";
      case CPlayerStateMP1::EItemType::Newborn:
        return "Artifact of Newborn";
      default:
        return "[unknown]";
    }
  }

  void RenderItemsDualColumn(CPlayerStateMP1 *playerState, const CPlayerStateMP1::EItemType *items, int start, int end) {
    ImGui::BeginGroup();
    // Render left group
    for (int i = start; i < end; i += 2) {
      RenderItemType(playerState, items[i]);
    }
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
    // Render right group
    for (int i = start + 1; i < end; i += 2) {
      RenderItemType(playerState, items[i]);
    }
    ImGui::EndGroup();
  }
}
