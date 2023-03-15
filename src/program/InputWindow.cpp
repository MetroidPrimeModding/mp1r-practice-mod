#include "InputWindow.hpp"
#include "imgui.h"

#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui_internal.h"
#include "patches.hpp"
#include "PlayerMenu.hpp"
#include "helpers/InputHelper.h"

struct InputDisplayInput {
  bool fromCPlayer;

  float leftX;
  float leftY;
  float rightX;
  float rightY;

  bool minus;
  bool plus;

  bool A;
  bool B;
  bool X;
  bool Y;

  bool L;
  bool R;
  float ZL;
  bool zlHeld;
  float ZR;
  bool zrHeld;

  bool dpadLeft;
  bool dpadRight;
  bool dpadUp;
  bool dpadDown;
};

namespace GUI {
  int framesSincePlayerInput;
  CFinalInput lastKnownCPlayerInput;
  double igt;

  EPlayerMovementState moveState;
  uint32_t bombJumpState;
  uint32_t isInHalfPipeMode;
  uint32_t isInHalfPipeModeInAir;
  uint32_t touchedHalfPipeRecently;

  void drawInput(InputDisplayInput &p1);
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

  void drawMoveState();

  void drawMonitorWindow() {
    if (!PATCH_CONFIG.OSD_showMonitor) {
      return;
    }
    {
      ImGui::SetNextWindowPos(ImVec2(200, 10), ImGuiCond_FirstUseEver);
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
      if (PATCH_CONFIG.OSD_showMoveState) {
        drawMoveState();
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

  void drawMoveState() {
    ImGui::Text("%s (%d)", stateToName(moveState), (uint32_t) moveState);
    if (isInHalfPipeMode) {
      ImGui::SameLine();
      ImGui::Text(" half pipe");
    }
    if (isInHalfPipeModeInAir) {
      ImGui::SameLine();
      ImGui::Text(" in air");
    }
    if (touchedHalfPipeRecently) {
      ImGui::SameLine();
      ImGui::Text(" recently");
    }
  }

  void drawLastKnownPlayerInput();

  void drawRawInput();

  void drawInputWindow() {
//    ImGui::SetNextWindowPos(ImVec2(400, 400), ImGuiCond_FirstUseEver, ImVec2(1, 1));
//    ImGui::Begin(
//        "Input", nullptr
//    );
//    ImGui::Text("TEST POS");
//    ImGui::Text("pos: %f %f", ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
//    ImGui::End();

//    if (framesSincePlayerInput == 0) {
//      drawLastKnownPlayerInput();
//    } else {
      drawRawInput();
//    }
    framesSincePlayerInput++;


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
//                    lastKnownCPlayerInput.GetDigitalHeld(control),
//                    lastKnownCPlayerInput.GetDigitalPressed(control),
//                    lastKnownCPlayerInput.GetDigitalReleased(control),
//                    lastKnownCPlayerInput.GetAnalog(control));
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

  void drawLastKnownPlayerInput() {
    InputDisplayInput p1{
      .fromCPlayer = true,

      .leftX = lastKnownCPlayerInput.GetAnalog(EControl::LEFT_STICK_X),
      .leftY = lastKnownCPlayerInput.GetAnalog(EControl::LEFT_STICK_Y),
      .rightX = lastKnownCPlayerInput.GetAnalog(EControl::RIGHT_STICK_X),
      .rightY = lastKnownCPlayerInput.GetAnalog(EControl::RIGHT_STICK_Y),

      .minus = lastKnownCPlayerInput.GetDigitalHeld(EControl::MINUS),
      .plus = lastKnownCPlayerInput.GetDigitalHeld(EControl::PLUS),

      .A = lastKnownCPlayerInput.GetDigitalHeld(EControl::A),
      .B = lastKnownCPlayerInput.GetDigitalHeld(EControl::B),
      .X = lastKnownCPlayerInput.GetDigitalHeld(EControl::X),
      .Y = lastKnownCPlayerInput.GetDigitalHeld(EControl::Y),

      .L = lastKnownCPlayerInput.GetDigitalHeld(EControl::L),
      .R = lastKnownCPlayerInput.GetDigitalHeld(EControl::R),
      .ZL = lastKnownCPlayerInput.GetAnalog(EControl::ZL),
      .zlHeld = lastKnownCPlayerInput.GetDigitalHeld(EControl::ZL),
      .ZR = lastKnownCPlayerInput.GetAnalog(EControl::ZR),
      .zrHeld = lastKnownCPlayerInput.GetDigitalHeld(EControl::ZR),

      .dpadLeft = lastKnownCPlayerInput.GetDigitalHeld(EControl::DPAD_LEFT),
      .dpadRight = lastKnownCPlayerInput.GetDigitalHeld(EControl::DPAD_RIGHT),
      .dpadUp = lastKnownCPlayerInput.GetDigitalHeld(EControl::DPAD_UP),
      .dpadDown = lastKnownCPlayerInput.GetDigitalHeld(EControl::DPAD_DOWN),
    };
    drawInput(p1);
  }

  void drawRawInput() {
    InputDisplayInput p1{
        .fromCPlayer = false,

        .leftX = InputHelper::getLeftStickX(),
        .leftY = InputHelper::getLeftStickY(),
        .rightX = InputHelper::getRightStickX(),
        .rightY = InputHelper::getRightStickY(),

        .minus = InputHelper::isHoldMinus(),
        .plus = InputHelper::isHoldPlus(),

        .A = InputHelper::isHoldA(),
        .B = InputHelper::isHoldB(),
        .X = InputHelper::isHoldX(),
        .Y = InputHelper::isHoldY(),

        .L = InputHelper::isHoldL(),
        .R = InputHelper::isHoldR(),
        .ZL = InputHelper::isHoldZL() ? 1.f : 0.f,
        .zlHeld = InputHelper::isHoldZL(),
        .ZR = InputHelper::isHoldZR() ? 1.f : 0.f,
        .zrHeld = InputHelper::isHoldZR(),

        .dpadLeft = InputHelper::isHoldPadLeft(),
        .dpadRight = InputHelper::isHoldPadRight(),
        .dpadUp = InputHelper::isHoldPadUp(),
        .dpadDown = InputHelper::isHoldPadDown(),
    };

    drawInput(p1);
  }

  void drawInput(InputDisplayInput &p1) {
    if (!PATCH_CONFIG.OSD_showInput) return;
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver, ImVec2(0, 0));
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
//    ImVec2 realPos = ImGui::GetWindowPos();

    float scale = PATCH_CONFIG.OSD_inputScale;
    ImVec2 p = ImGui::GetCursorScreenPos();

    float stickThickness = 3 * scale;
    float leftStickRadius = 14 * scale;
    ImVec2 leftStickCenter = p + ImVec2(14, 35) * scale;
    float dpadRadius = 12 * scale;
    float dpadWidth = 5 * scale;
    ImVec2 dpadCenter = p + ImVec2(40, 60) * scale;
    float rightStickRadius = 14 * scale;
    ImVec2 rightStickCenter = p + ImVec2(80, 60) * scale;
    float plusButtonRadius = 3 * scale;
    ImVec2 plusButtonCenter = p + ImVec2(45, 30) * scale;
    float minusButtonRadius = 3 * scale;
    ImVec2 minusButtonCenter = p + ImVec2(75, 30) * scale;

    ImVec2 rightCrossCenter = leftStickCenter + ImVec2(90, 0) * scale;
    float aButtonRadius = 5 * scale;
    ImVec2 aButtonCenter = rightCrossCenter + (ImVec2(10, 0)) * scale;
    float bButtonRadius = 5 * scale;
    ImVec2 bButtonCenter = rightCrossCenter + (ImVec2(0, 10)) * scale;
    float xButtonRadius = 5 * scale;
    ImVec2 xButtonCenter = rightCrossCenter + (ImVec2(0, -10)) * scale;
    float yButtonRadius = 5 * scale;
    ImVec2 yButtonCenter = rightCrossCenter + (ImVec2(-10, 0)) * scale;
    float triggerWidth = leftStickRadius * 2;
    float triggerHeight = 4 * scale;
    ImVec2 zlCenter = leftStickCenter + ImVec2(0, -35) * scale;
    ImVec2 lCenter = leftStickCenter + ImVec2(0, -25) * scale;
    ImVec2 zrCenter = ImVec2(rightCrossCenter.x, zlCenter.y);
    ImVec2 rCenter = ImVec2(rightCrossCenter.x, lCenter.y);
    float halfTriggerWidth = triggerWidth / 2;


    ImVec2 padding = ImVec2(4, 4) * scale;
    ImVec2 topLeft = zlCenter - ImVec2(halfTriggerWidth, 0) - padding;
    ImVec2 bottomRight = ImVec2(aButtonCenter.x + aButtonRadius, rightStickCenter.y + rightStickRadius) + padding;
    ImVec2 size = bottomRight - topLeft;

    // Save some perf, potentially
    int circle_segments;
    if (scale < 2) {
      circle_segments = 24;
    } else if (scale < 4) {
      circle_segments = 48;
    } else {
      circle_segments = 64;
    }

    constexpr ImU32 bg_color = IM_COL32(0, 0, 0, 150);
    constexpr ImU32 gray = IM_COL32(190, 190, 190, 255);
    // raw input is blue
    constexpr ImU32 raw_input = IM_COL32(0, 80, 0, 255);
    // cplayer input is blue?
//    constexpr ImU32 cplayer_input = IM_COL32(0, 0, 150, 255);

    ImU32 active_color = raw_input;
//    // Prevents flicker
//    if (p1.fromCPlayer || framesSincePlayerInput < 3) {
//      active_color = cplayer_input;
//    } else {
//      active_color = raw_input;
//    }

    // bg
    {
      dl->AddRectFilled(topLeft, bottomRight, bg_color);
    }

    // left stick
    {
      dl->AddCircleFilled(
          leftStickCenter, leftStickRadius,
          gray,
          circle_segments
      );
      float x = p1.leftX;
      float y = -p1.leftY;
      dl->AddLine(
          leftStickCenter,
          leftStickCenter + ImVec2(x * leftStickRadius, y * leftStickRadius),
          active_color,
          stickThickness
      );
    }

    // right stick
    {
      dl->AddCircleFilled(
          rightStickCenter, rightStickRadius,
          gray,
          circle_segments
      );
      float x = p1.rightX;
      float y = -p1.rightY;
      dl->AddLine(
          rightStickCenter,
          rightStickCenter + ImVec2(x * rightStickRadius, y * rightStickRadius),
          active_color,
          stickThickness
      );
    }

    // dpad
    {
      float halfWidth = dpadWidth / 2;
      dl->AddRectFilled(
          dpadCenter + ImVec2(-halfWidth, -dpadRadius),
          dpadCenter + ImVec2(halfWidth, dpadRadius),
          gray
      );

      dl->AddRectFilled(
          dpadCenter + ImVec2(-dpadRadius, -halfWidth),
          dpadCenter + ImVec2(dpadRadius, halfWidth),
          gray
      );

      if (p1.dpadUp) {
        dl->AddRectFilled(
            dpadCenter + ImVec2(-halfWidth, -dpadRadius),
            dpadCenter + ImVec2(halfWidth, -dpadRadius / 2),
            active_color
        );
      }

      if (p1.dpadDown) {
        dl->AddRectFilled(
            dpadCenter + ImVec2(-halfWidth, dpadRadius),
            dpadCenter + ImVec2(halfWidth, dpadRadius / 2),
            active_color
        );
      }

      if (p1.dpadLeft) {
        dl->AddRectFilled(
            dpadCenter + ImVec2(-dpadRadius, -halfWidth),
            dpadCenter + ImVec2(-dpadRadius / 2, halfWidth),
            active_color
        );
      }

      if (p1.dpadRight) {
        dl->AddRectFilled(
            dpadCenter + ImVec2(dpadRadius, -halfWidth),
            dpadCenter + ImVec2(dpadRadius / 2, halfWidth),
            active_color
        );
      }
    }

    // buttons
    {
      // plus
      dl->AddCircleFilled(
          plusButtonCenter, plusButtonRadius,
          p1.plus ? active_color : gray,
          circle_segments
      );

      // minus
      dl->AddCircleFilled(
          minusButtonCenter, minusButtonRadius,
          p1.minus ? active_color : gray,
          circle_segments
      );

      // a
      dl->AddCircleFilled(
          aButtonCenter, aButtonRadius,
          p1.A ? active_color : gray,
          circle_segments
      );

      //b
      dl->AddCircleFilled(
          bButtonCenter, bButtonRadius,
          p1.B ? active_color : gray,
          circle_segments
      );

      //x
      dl->AddCircleFilled(
          xButtonCenter, xButtonRadius,
          p1.X ? active_color : gray,
          circle_segments
      );

      //y
      dl->AddCircleFilled(
          yButtonCenter, yButtonRadius,
          p1.Y ? active_color : gray,
          circle_segments
      );
    }

    // triggers
    {
      ImVec2 zlStart = zlCenter - ImVec2(halfTriggerWidth, 0);
      ImVec2 zlEnd = zlCenter + ImVec2(halfTriggerWidth, triggerHeight);
      float zlValue = triggerWidth * p1.ZL;

      dl->AddRectFilled(
          zlStart, zlStart + ImVec2(zlValue, triggerHeight),
          p1.zlHeld ? active_color : gray
      );
      dl->AddRectFilled(
          zlStart + ImVec2(zlValue, 0), zlEnd,
          gray
      );

      ImVec2 lStart = lCenter - ImVec2(halfTriggerWidth, 0);
      ImVec2 lEnd = lCenter + ImVec2(halfTriggerWidth, triggerHeight);
      float lValue = p1.L ? triggerWidth : 0;

      dl->AddRectFilled(
          lStart, lStart + ImVec2(lValue, triggerHeight),
          p1.L ? active_color : gray
      );
      dl->AddRectFilled(
          lStart + ImVec2(lValue, 0), lEnd,
          gray
      );

      ImVec2 zrStart = zrCenter - ImVec2(halfTriggerWidth, 0);
      ImVec2 zrEnd = zrCenter + ImVec2(halfTriggerWidth, triggerHeight);
      float zrValue = triggerWidth * p1.ZR;

      dl->AddRectFilled(
          zrEnd - ImVec2(zrValue, triggerHeight), zrEnd,
          p1.zrHeld ? active_color : gray
      );
      dl->AddRectFilled(
          zrStart, zrEnd - ImVec2(zrValue, 0),
          gray
      );

      ImVec2 rStart = rCenter - ImVec2(halfTriggerWidth, 0);
      ImVec2 rEnd = rCenter + ImVec2(halfTriggerWidth, triggerHeight);
      float rValue = p1.R ? triggerWidth : 0;

      dl->AddRectFilled(
          rEnd - ImVec2(rValue, triggerHeight), rEnd,
          p1.R ? active_color : gray
      );
      dl->AddRectFilled(
          rStart, rEnd - ImVec2(rValue, 0),
          gray
      );
    }

    ImGui::Dummy(size);
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
    auto &vel = GUI::lastKnownVelocity;

    float x = vel.x;
    float y = vel.y;
    float z = vel.z;
    float h = sqrtf(x * x + z * z);

    ImGui::Text("Vel: %5.2fx %5.2fy %5.2fz %5.2fh", x, y, z, h);
  }
}