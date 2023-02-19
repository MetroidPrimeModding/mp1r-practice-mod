#include "PlayerMenu.hpp"
#include "InventoryMenu.hpp"
#include "patches.hpp"
#include "imgui.h"
#include "prime/CPlayerMP1.hpp"

#include <numeric>

#define BITFIELD_CHECKBOX(label, bf, ...)                                                                              \
  {                                                                                                                    \
    bool b = (bf);                                                                                                     \
    if (ImGui::Checkbox(label, &b)) {                                                                                  \
      (bf) = b;                                                                                                        \
      __VA_ARGS__                                                                                                      \
    }                                                                                                                  \
  }

namespace GUI {
  bool hasDesiredPositionData = false;

  CTransform4f desiredTransform = CTransform4f::Identity();
  CTransform4f lastKnownTransform = CTransform4f::Identity();
  CVector3f desiredVelocity{};
  CVector3f lastKnownVelocity{};
  CAxisAngle desiredAngularVelocity{};
  CAxisAngle lastKnownAngularVelocity{};

  CTransform4f savedPos{CTransform4f::Identity()};
//  CVector3f savedVelocity{};
//  CAxisAngle savedAngularVelocity{};
//  u32 savedWorldAssetID{0};
//  u32 savedAreaAssetID{0};

  void drawPlayerMenu() {
//    CStateManager *stateManager = mostRecentStateManager;
//    if (stateManager == nullptr) return;
//    auto gameLogic = stateManager->GameLogic();
//    if (gameLogic == nullptr) return;
//    auto player = gameLogic->PlayerActor();
//    if (player == nullptr) return;
//    CPlayerState *playerState = stateManager->GetPlayerState();

    ImGuiSliderFlags flags = ImGuiSliderFlags_None
                             | ImGuiSliderFlags_NoRoundToFormat;
    if (ImGui::TreeNode("Player")) {
      ImGui::Checkbox("Invulnerable", &PATCH_CONFIG.invulnerable);

      ImGui::Text("Saved position:");
      ImGui::Text("%3.4f %3.4f %3.4f", savedPos.x, savedPos.y, savedPos.z);
      if (ImGui::Button("Save")) {
        savePos();
      }
      ImGui::SameLine();
      if (ImGui::Button("Load")) {
        loadPos();
      }
//      ImGui::SameLine();

      float xyz[3] = {
          lastKnownTransform.x,
          lastKnownTransform.y,
          lastKnownTransform.z
      };

      if (ImGui::DragFloat3("##pos", xyz, 1.f, -FLT_MAX, FLT_MAX, "%.3f", flags)) {
        desiredTransform = lastKnownTransform;
        desiredTransform.x = xyz[0];
        desiredTransform.y = xyz[1];
        desiredTransform.z = xyz[2];
        desiredVelocity = lastKnownVelocity;
        desiredAngularVelocity = lastKnownAngularVelocity;
        hasDesiredPositionData = true;
      }
      BITFIELD_CHECKBOX("Allow position edit with ZR + sticks", PATCH_CONFIG.pos_edit);
      ImGui::Text("Hold ZL for faster movement\n");

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
    desiredTransform = savedPos;
    desiredVelocity = {0, 0, 0};
    desiredAngularVelocity = {0, 0, 0};
    hasDesiredPositionData = true;
  }

  void savePos() {
//    savedPos = *player->getTransform();
//    savedVelocity = *player->GetVelocity();
//    savedAngularVelocity = *player->GetAngularVelocity();
    savedPos = lastKnownTransform;
//    savedVelocity = lastKnownVelocity;
//    savedAngularVelocity = lastKnownAngularVelocity;
  }
}

