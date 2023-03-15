#pragma once

#include "nn/hid.h"

class InputHelper {

public:
  static void updatePadState();

  static void setPort(ulong port) { selectedPort = port; }

  static void initKBM();

  // controller inputs

  static bool isButtonHold(nn::hid::NpadButton button);

  static bool isButtonPress(nn::hid::NpadButton button);

  static bool isButtonRelease(nn::hid::NpadButton button);

  // input disabling

  static bool isReadInputs() { return isReadInput; }

  static bool isInputToggled() { return toggleInput; }

  // keyboard key presses

  static bool isKeyHold(nn::hid::KeyboardKey key);

  static bool isKeyPress(nn::hid::KeyboardKey key);

  static bool isKeyRelease(nn::hid::KeyboardKey key);

  // mouse inputs

  static bool isMouseHold(nn::hid::MouseButton button);

  static bool isMousePress(nn::hid::MouseButton button);

  static bool isMouseRelease(nn::hid::MouseButton button);

  // mouse coordinate getters

  static void getMouseCoords(float *x, float *y);

  static void getScrollDelta(float *x, float *y);

  // specific button funcs

  static bool isHoldA() { return isButtonHold(nn::hid::NpadButton::A); }

  static bool isPressA() { return isButtonPress(nn::hid::NpadButton::A); }

  static bool isReleaseA() { return isButtonRelease(nn::hid::NpadButton::A); }

  static bool isHoldB() { return isButtonHold(nn::hid::NpadButton::B); }

  static bool isPressB() { return isButtonPress(nn::hid::NpadButton::B); }

  static bool isReleaseB() { return isButtonRelease(nn::hid::NpadButton::B); }

  static bool isHoldX() { return isButtonHold(nn::hid::NpadButton::X); }

  static bool isPressX() { return isButtonPress(nn::hid::NpadButton::X); }

  static bool isReleaseX() { return isButtonRelease(nn::hid::NpadButton::X); }

  static bool isHoldY() { return isButtonHold(nn::hid::NpadButton::Y); }

  static bool isPressY() { return isButtonPress(nn::hid::NpadButton::Y); }

  static bool isReleaseY() { return isButtonRelease(nn::hid::NpadButton::Y); }

  static bool isHoldL() { return isButtonHold(nn::hid::NpadButton::L); }

  static bool isPressL() { return isButtonPress(nn::hid::NpadButton::L); }

  static bool isReleaseL() { return isButtonRelease(nn::hid::NpadButton::L); }

  static bool isHoldR() { return isButtonHold(nn::hid::NpadButton::R); }

  static bool isPressR() { return isButtonPress(nn::hid::NpadButton::R); }

  static bool isReleaseR() { return isButtonRelease(nn::hid::NpadButton::R); }

  static bool isHoldZL() { return isButtonHold(nn::hid::NpadButton::ZL); }

  static bool isPressZL() { return isButtonPress(nn::hid::NpadButton::ZL); }

  static bool isReleaseZL() { return isButtonRelease(nn::hid::NpadButton::ZL); }

  static bool isHoldZR() { return isButtonHold(nn::hid::NpadButton::ZR); }

  static bool isPressZR() { return isButtonPress(nn::hid::NpadButton::ZR); }

  static bool isReleaseZR() { return isButtonRelease(nn::hid::NpadButton::ZR); }

  static bool isHoldPadLeft() { return isButtonHold(nn::hid::NpadButton::Left); }

  static bool isPressPadLeft() { return isButtonPress(nn::hid::NpadButton::Left); }

  static bool isReleasePadLeft() { return isButtonRelease(nn::hid::NpadButton::Left); }

  static bool isHoldPadRight() { return isButtonHold(nn::hid::NpadButton::Right); }

  static bool isPressPadRight() { return isButtonPress(nn::hid::NpadButton::Right); }

  static bool isReleasePadRight() { return isButtonRelease(nn::hid::NpadButton::Right); }

  static bool isHoldPadUp() { return isButtonHold(nn::hid::NpadButton::Up); }

  static bool isPressPadUp() { return isButtonPress(nn::hid::NpadButton::Up); }

  static bool isReleasePadUp() { return isButtonRelease(nn::hid::NpadButton::Up); }

  static bool isHoldPadDown() { return isButtonHold(nn::hid::NpadButton::Down); }

  static bool isPressPadDown() { return isButtonPress(nn::hid::NpadButton::Down); }

  static bool isReleasePadDown() { return isButtonRelease(nn::hid::NpadButton::Down); }

  static bool isPressLeftStick() { return isButtonPress(nn::hid::NpadButton::StickL); }

  static bool isHoldLeftStick() { return isButtonHold(nn::hid::NpadButton::StickL); }

  static bool isPressRightStick() { return isButtonPress(nn::hid::NpadButton::StickR); }

  static bool isHoldRightStick() { return isButtonHold(nn::hid::NpadButton::StickR); }

  static bool isHoldPlus() { return isButtonHold(nn::hid::NpadButton::Plus); }

  static bool isPressPlus() { return isButtonPress(nn::hid::NpadButton::Plus); }

  static bool isReleasePlus() { return isButtonRelease(nn::hid::NpadButton::Plus); }

  static bool isHoldMinus() { return isButtonHold(nn::hid::NpadButton::Minus); }

  static bool isPressMinus() { return isButtonPress(nn::hid::NpadButton::Minus); }

  static bool isReleaseMinus() { return isButtonRelease(nn::hid::NpadButton::Minus); }

  // analog

  static float getLeftStickX() { return ((float) curControllerState.mAnalogStickL.X) / 32768.f; }
  static float getLeftStickY() { return ((float) curControllerState.mAnalogStickL.Y) / 32768.f; }
  static float getLeftStickLeft() { float x = getLeftStickX(); return x < 0 ? -x : 0; }
  static float getLeftStickRight() { float x = getLeftStickX(); return x > 0 ? x : 0; }
  static float getLeftStickUp() { float y = getLeftStickY(); return y > 0 ? y : 0; }
  static float getLeftStickDown() { float y = getLeftStickY(); return y < 0 ? -y : 0; }

  static float getRightStickX() { return ((float) curControllerState.mAnalogStickR.X) / 32768.f; }
  static float getRightStickY() { return ((float) curControllerState.mAnalogStickR.Y) / 32768.f; }
  static float getRightStickLeft() { float x = getRightStickX(); return x < 0 ? -x : 0; }
  static float getRightStickRight() { float x = getRightStickX(); return x > 0 ? x : 0; }
  static float getRightStickUp() { float y = getRightStickY(); return y > 0 ? y : 0; }
  static float getRightStickDown() { float y = getRightStickY(); return y < 0 ? -y : 0; }




private:
  static bool tryGetContState(nn::hid::NpadBaseState *state, ulong port);

  static char getKeyValue(nn::hid::KeyboardKey key, bool isUpper, bool isModifier);

  static nn::hid::NpadBaseState curControllerState;
  static nn::hid::NpadBaseState prevControllerState;

  static nn::hid::KeyboardState curKeyboardState;
  static nn::hid::KeyboardState prevKeyboardState;

  static nn::hid::MouseState curMouseState;
  static nn::hid::MouseState prevMouseState;

  static ulong selectedPort;

  static bool isReadInput;
public:
  static bool toggleInput;
};