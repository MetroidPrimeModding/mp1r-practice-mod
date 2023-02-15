#include "InputWindow.hpp"
#include "imgui.h"

#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui_internal.h"
#include "patches.hpp"
#include "PlayerMenu.hpp"

namespace GUI {
  bool hasInput;
  CFinalInput lastKnownInput;
  double igt;

  void drawInput(CFinalInput &p1);
  void drawPos();
  void drawVelocity();

  void drawIGT() {
    double time = igt;
    int ms = (int) (time * 1000.0) % 1000;
    int seconds = (int) time % 60;
    int minutes = ((int) time / 60) % 60;
    int hours = ((int) time / 60 / 60) % 60;
    ImGui::Text("%02d:%02d:%02d.%03d", hours, minutes, seconds, ms);
  }

  void drawMonitorWindow() {
    if (!PATCH_CONFIG.OSD_showMonitor) {
      return;
    }
    {
      ImGui::SetNextWindowPos(ImVec2(630, 10), ImGuiCond_Once, ImVec2(1, 0));
      ImGui::Begin(
          "Monitor", nullptr,
          ImGuiWindowFlags_NoResize |
          ImGuiWindowFlags_AlwaysAutoResize |
          ImGuiWindowFlags_NoTitleBar |
//          ImGuiWindowFlags_NoInputs |
//          ImGuiWindowFlags_NoNavInputs |
//          ImGuiWindowFlags_NoNavFocus |
//          ImGuiWindowFlags_NoNav |
          ImGuiWindowFlags_NoFocusOnAppearing |
//          ImGuiWindowFlags_NoMove |
//          ImGuiWindowFlags_NoDecoration |
          //        ImGuiWindowFlags_NoBackground |
          ImGuiFocusedFlags_None
      );
      if (PATCH_CONFIG.OSD_showIGT) {
        drawIGT();
      }
//      drawRoomTime();
      if (PATCH_CONFIG.OSD_showPos) {
        drawPos();
      }
      if (PATCH_CONFIG.OSD_showVelocity) {
        drawVelocity();
      }
//      if (SETTINGS.OSD_showRotationalVelocity) {
//        drawRotationalVelocity();
//      }
//      if (SETTINGS.OSD_showFrameTime) {
//        drawFrameTime();
//      }
//      if (SETTINGS.OSD_showMemoryGraph || SETTINGS.OSD_showMemoryInfo) {
//        drawMemoryUsage();
//      }

      ImGui::End();
    }
  }

  void drawInputWindow() {
//    ImGui::SetNextWindowPos(ImVec2(400, 400), ImGuiCond_FirstUseEver, ImVec2(1, 1));
//    ImGui::Begin(
//        "Input", nullptr
//    );
//    ImGui::Text("TEST POS");
//    ImGui::Text("pos: %f %f", ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
//    ImGui::End();

    if (!hasInput) return;
    drawInput(lastKnownInput);

//    ImGui::SetNextWindowSizeConstraints(ImVec2(200, 100), ImVec2(700, 700));
//    if (ImGui::Begin("Input", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize)) {
//      ImGui::Text("Input h p r a");
//      ImGui::Text("pos: %f %f", ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
//
//      int onRow = 0;
//      for (int i = 0; i < 0x88; i++) {
//        auto control = static_cast<EControl>(i);
//        ImGui::Text("%x: %d %d %d %f",
//                    i,
//                    lastKnownInput.GetDigitalHeld(control),
//                    lastKnownInput.GetDigitalPressed(control),
//                    lastKnownInput.GetDigitalReleased(control),
//                    lastKnownInput.GetAnalog(control));
//        onRow++;
//        if (onRow < 4) {
//          ImGui::SameLine();
//        } else {
//          onRow = 0;
//        }
//      }
//
//      ImGui::End();
//    }
  }

  void drawInput(CFinalInput &p1) {
    if (!PATCH_CONFIG.show_input) return;
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once, ImVec2(0, 0));
    ImGui::Begin(
        "Input", nullptr,
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoTitleBar |
//        ImGuiWindowFlags_NoInputs |
//        ImGuiWindowFlags_NoNavInputs |
//        ImGuiWindowFlags_NoNavFocus |
//        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoBackground |
        ImGuiFocusedFlags_None // just for conveneint commenting in/out
    );

    ImDrawList *dl = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetCursorScreenPos();

    constexpr float stickThickness = 3;
    constexpr float leftStickRadius = 15;
    ImVec2 leftStickCenter = p + ImVec2(15, 35);
    constexpr float dpadRadius = 10;
    constexpr float dpadWidth = 4;
    ImVec2 dpadCenter = p + ImVec2(40, 60);
    constexpr float rightStickRadius = 10;
    ImVec2 rightStickCenter = p + ImVec2(80, 60);
    constexpr float startButtonRadius = 4;
    ImVec2 startButtonCenter = p + ImVec2(60, 40);
    constexpr float aButtonRadius = 8;
    ImVec2 aButtonCenter = p + ImVec2(110, 38);
    constexpr float bButtonRadius = 4;
    ImVec2 bButtonCenter = aButtonCenter + ImVec2(-12, 8);
    constexpr float xButtonRadius = 4;
    ImVec2 xButtonCenter = aButtonCenter + ImVec2(12, -8);
    constexpr float yButtonRadius = 4;
    ImVec2 yButtonCenter = aButtonCenter + ImVec2(-6, -12);
    constexpr float triggerWidth = leftStickRadius * 2;
    constexpr float triggerHeight = 4;
    ImVec2 zlCenter = leftStickCenter + ImVec2(0, -35);
    ImVec2 lCenter = leftStickCenter + ImVec2(0, -25);
    ImVec2 zrCenter = ImVec2(aButtonCenter.x, zlCenter.y);
    ImVec2 rCenter = ImVec2(aButtonCenter.x, lCenter.y);


    constexpr ImU32 stickGray = IM_COL32(150, 150, 150, 255);
    constexpr ImU32 darkGray = IM_COL32(60, 60, 60, 255);
    constexpr ImU32 red = IM_COL32(255, 0, 0, 255);
    constexpr ImU32 green = IM_COL32(0, 255, 0, 255);

    // left stick
    {
      dl->AddCircleFilled(
          leftStickCenter, leftStickRadius,
          stickGray,
          8
      );
      float x = p1.GetAnalog(EControl::LEFT_STICK_X);
      float y = -p1.GetAnalog(EControl::LEFT_STICK_Y);
      dl->AddLine(
          leftStickCenter,
          leftStickCenter + ImVec2(x * leftStickRadius, y * leftStickRadius),
          red,
          stickThickness
      );
    }

    // right stick
    {
      dl->AddCircleFilled(
          rightStickCenter, rightStickRadius,
          stickGray,
          8
      );
      float x = p1.GetAnalog(EControl::RIGHT_STICK_X);
      float y = -p1.GetAnalog(EControl::RIGHT_STICK_Y);
      dl->AddLine(
          rightStickCenter,
          rightStickCenter + ImVec2(x * rightStickRadius, y * rightStickRadius),
          red,
          stickThickness
      );
    }

    // dpad
    {
      constexpr float halfWidth = dpadWidth / 2;
      dl->AddRectFilled(
          dpadCenter + ImVec2(-halfWidth, -dpadRadius),
          dpadCenter + ImVec2(halfWidth, dpadRadius),
          stickGray
      );

      dl->AddRectFilled(
          dpadCenter + ImVec2(-dpadRadius, -halfWidth),
          dpadCenter + ImVec2(dpadRadius, halfWidth),
          stickGray
      );

      if (p1.GetDigitalHeld(EControl::DPAD_UP)) {
        dl->AddRectFilled(
            dpadCenter + ImVec2(-halfWidth, -dpadRadius),
            dpadCenter + ImVec2(halfWidth, -dpadRadius / 2),
            red
        );
      }

      if (p1.GetDigitalHeld(EControl::DPAD_DOWN)) {
        dl->AddRectFilled(
            dpadCenter + ImVec2(-halfWidth, dpadRadius),
            dpadCenter + ImVec2(halfWidth, dpadRadius / 2),
            red
        );
      }

      if (p1.GetDigitalHeld(EControl::DPAD_LEFT)) {
        dl->AddRectFilled(
            dpadCenter + ImVec2(-dpadRadius, -halfWidth),
            dpadCenter + ImVec2(-dpadRadius / 2, halfWidth),
            red
        );
      }

      if (p1.GetDigitalHeld(EControl::DPAD_RIGHT)) {
        dl->AddRectFilled(
            dpadCenter + ImVec2(dpadRadius, -halfWidth),
            dpadCenter + ImVec2(dpadRadius / 2, halfWidth),
            red
        );
      }
    }

    // buttons
    {
      // start
      dl->AddCircleFilled(
          startButtonCenter, startButtonRadius,
          p1.GetDigitalHeld(EControl::MINUS) ? red : stickGray,
          16
      );

      // a
      dl->AddCircleFilled(
          aButtonCenter, aButtonRadius,
          p1.GetDigitalHeld(EControl::A) ? green : stickGray,
          16
      );

      //b
      dl->AddCircleFilled(
          bButtonCenter, bButtonRadius,
          p1.GetDigitalHeld(EControl::B) ? red : stickGray,
          16
      );

      //x
      dl->AddCircleFilled(
          xButtonCenter, xButtonRadius,
          p1.GetDigitalHeld(EControl::X) ? red : stickGray,
          16
      );

      //y
      dl->AddCircleFilled(
          yButtonCenter, yButtonRadius,
          p1.GetDigitalHeld(EControl::Y) ? red : stickGray,
          16
      );
    }

    // triggers
    {
      float halfTriggerWidth = triggerWidth / 2;
      ImVec2 zlStart = zlCenter - ImVec2(halfTriggerWidth, 0);
      ImVec2 zlEnd = zlCenter + ImVec2(halfTriggerWidth, triggerHeight);
      float zlValue = triggerWidth * p1.GetAnalog(EControl::ZL);

      dl->AddRectFilled(
          zlStart, zlStart + ImVec2(zlValue, triggerHeight),
          p1.GetDigitalHeld(EControl::ZL) ? red : stickGray
      );
      dl->AddRectFilled(
          zlStart + ImVec2(zlValue, 0), zlEnd,
          darkGray
      );

      ImVec2 lStart = lCenter - ImVec2(halfTriggerWidth, 0);
      ImVec2 lEnd = lCenter + ImVec2(halfTriggerWidth, triggerHeight);
      float lValue = triggerWidth * p1.GetAnalog(EControl::L);

      dl->AddRectFilled(
          lStart, lStart + ImVec2(lValue, triggerHeight),
          p1.GetDigitalHeld(EControl::L) ? red : stickGray
      );
      dl->AddRectFilled(
          lStart + ImVec2(lValue, 0), lEnd,
          darkGray
      );

      ImVec2 zrStart = zrCenter - ImVec2(halfTriggerWidth, 0);
      ImVec2 zrEnd = zrCenter + ImVec2(halfTriggerWidth, triggerHeight);
      float zrValue = triggerWidth * p1.GetAnalog(EControl::ZR);

      dl->AddRectFilled(
          zrEnd - ImVec2(zrValue, triggerHeight), zrEnd,
          p1.GetDigitalHeld(EControl::ZR) ? red : stickGray
      );
      dl->AddRectFilled(
          zrStart, zrEnd - ImVec2(zrValue, 0),
          darkGray
      );

      ImVec2 rStart = rCenter - ImVec2(halfTriggerWidth, 0);
      ImVec2 rEnd = rCenter + ImVec2(halfTriggerWidth, triggerHeight);
      float rValue = triggerWidth * p1.GetAnalog(EControl::R);

      dl->AddRectFilled(
          rEnd - ImVec2(rValue, triggerHeight), rEnd,
          p1.GetDigitalHeld(EControl::ZR) ? red : stickGray
      );
      dl->AddRectFilled(
          rStart, rEnd - ImVec2(rValue, 0),
          darkGray
      );
    }

    ImGui::Dummy(ImVec2(130, 80));
    ImGui::End();
  }

  void drawPos() {
    auto pos = GUI::lastKnownTransform;
    float x = pos.x;
    float y = pos.y;
    float z = pos.z;
    ImGui::Text("Pos: %7.2fx %7.2fy %7.2fz", x, y, z);
  }

  void drawVelocity() {
    auto vel = GUI::lastKnownVelocity;

    float x = vel.x;
    float y = vel.y;
    float z = vel.z;
    float h = sqrtf(x * x + y * y);

    ImGui::Text("Vel: %5.2fx %5.2fy %5.2fz %5.2fh", x, y, z, h);
  }
}