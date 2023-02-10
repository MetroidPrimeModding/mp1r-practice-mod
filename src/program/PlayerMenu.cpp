

#include "PlayerMenu.hpp"
#include "InventoryMenu.hpp"
#include "patches.hpp"
#include "imgui.h"

namespace GUI {
  CTransform4f savedPos{CTransform4f::Identity()};
  CVector3f savedVelocity{};
  CVector3f savedAngularVelocity{};
//  u32 savedWorldAssetID{0};
//  u32 savedAreaAssetID{0};

  void drawPlayerMenu() {
    CStateManager *stateManager = mostRecentStateManager;
    if (stateManager == nullptr) return;
    auto gameLogic = stateManager->GameLogic();
    if (gameLogic == nullptr) return;
    auto player = gameLogic->PlayerActor();
    if (player == nullptr) return;
//    CPlayerState *playerState = stateManager->GetPlayerState();

    ImGuiSliderFlags flags = ImGuiSliderFlags_None
                             | ImGuiSliderFlags_NoRoundToFormat;
    if (ImGui::TreeNode("Player")) {
      ImGui::Text("Saved position:");
//      if (ImGui::Button("Save")) {
//        savePos();
//      }
//      ImGui::SameLine();
//      if (ImGui::Button("Load")) {
//        loadPos();
//      }
      ImGui::SameLine();

      float xyz[3] = {
          player->GetTransform().x,
          player->GetTransform().y,
          player->GetTransform().z
      };

      ImGui::DragFloat3("##pos", xyz, 1.f, -FLT_MAX, FLT_MAX, "%.3f", flags);
//      CTransform4f newTransform = player->GetTransform();
//      newTransform.x = xyz[0];
//      newTransform.y = xyz[1];
//      newTransform.z = xyz[2];
//      player->SetTransform(newTransform);

//      if (ImGui::Button("IS on")) {
//        player->GetAngularVelocity()->x = NAN;
//        player->GetAngularVelocity()->y = NAN;
//        player->GetAngularVelocity()->z = NAN;
//      }
//      ImGui::SameLine();
//      ImGui::Text("Use while morphed");
//
//      auto *bobber = player->getCameraBobber();
//      if (ImGui::Button("Lightshow fix")) {
//        player->GetAngularVelocity()->x = 0;
//        player->GetAngularVelocity()->y = 0;
//        player->GetAngularVelocity()->z = 0;
//
//
//        *(bobber->getTargetBobMagnitude()) = 0;
//        *(bobber->getBobMagnitude()) = 0;
//        *(bobber->getBobTimescale()) = 0;
//        *(bobber->getBobTime()) = 0;
//        bobber->getCameraBobTransform()->x = 0;
//        bobber->getCameraBobTransform()->y = 0;
//        bobber->getCameraBobTransform()->z = 0;
//      }
//
//      ImGui::SameLine();
//      ImGui::Text("Use while unmorphed");
//
//      bool floaty = player->getFluidCounter() >= 1 && *player->getDepthUnderWater() >= 2;
//      if (ImGui::Checkbox("Floaty", &floaty)) {
//        if (floaty) {
//          player->setFluidCounter(1);
//          *player->getDepthUnderWater() = 20;
//        } else {
//          player->setFluidCounter(0);
//          *player->getDepthUnderWater() = 0;
//        };
//      }
//
//      int fluidCounter = (int) player->getFluidCounter();
//      if (ImGui::DragInt("Water box count", &fluidCounter, 1, 0, 0b11)) {
//        player->setFluidCounter((u32) fluidCounter);
//      }
//      ImGui::DragFloat("Water depth", player->getDepthUnderWater(), 1.f, -FLT_MAX, FLT_MAX, "%.3f", flags);

      ImGui::TreePop();
    }
  }

  void loadPos() {
//    CStateManager *stateManager = CStateManager_INSTANCE;
//    CPlayerMP1 *player = stateManager->Player();
//
//    *player->getTransform() = savedPos;
//    *player->GetVelocity() = savedVelocity;
//    *player->GetAngularVelocity() = savedAngularVelocity;
  }

  void savePos() {
//    CStateManager *stateManager = CStateManager_INSTANCE;
//    CPlayerMP1 *player = stateManager->Player();
//
//    CGameGlobalObjects *globals = ((CGameGlobalObjects *) 0x80457798);
//    CGameState *gameState = globals->getGameState();
//
//
//    u32 currentWorldAssetID = gameState->MLVL();
//    u32 currentAreaAssetID = 0;
//    CWorld *world = stateManager->GetWorld();
//    if (!world) return;
//    currentAreaAssetID = world->areas()->ptr[gameState->CurrentWorldState().x4_areaId.id].ptr->IGetAreaAssetId();
//
//    savedPos = *player->getTransform();
//    savedVelocity = *player->GetVelocity();
//    savedAngularVelocity = *player->GetAngularVelocity();
//    savedWorldAssetID = currentWorldAssetID;
//    savedAreaAssetID = currentAreaAssetID;
  }
}

