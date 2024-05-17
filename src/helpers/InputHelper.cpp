#include "InputHelper.h"
#include "diag/assert.hpp"
#include "logger/Logger.hpp"

static const char *styleNames[] = {
    "Pro Controller",
    "Joy-Con controller in handheld mode",
    "Joy-Con controller in dual mode",
    "Joy-Con left controller in single mode",
    "Joy-Con right controller in single mode",
    "GameCube controller",
    "Poké Ball Plus controller",
    "NES/Famicom controller",
    "NES/Famicom controller in handheld mode",
    "SNES controller",
    "N64 controller",
    "Sega Genesis controller",
    "generic external controller",
    "generic controller",
};

nn::hid::NpadBaseState InputHelper::prevControllerState{};
nn::hid::NpadBaseState InputHelper::curControllerState{};

nn::hid::KeyboardState InputHelper::curKeyboardState{};
nn::hid::KeyboardState InputHelper::prevKeyboardState{};

nn::hid::MouseState InputHelper::curMouseState{};
nn::hid::MouseState InputHelper::prevMouseState{};

nn::hid::TouchScreenState<1> InputHelper::curTouchState{};
nn::hid::TouchScreenState<1> InputHelper::prevTouchState{};

ulong InputHelper::selectedPort = -1;
bool InputHelper::isReadInput = true;
bool InputHelper::toggleInput = false;

const char *getStyleName(nn::hid::NpadStyleSet style) {

  u32 index = -1u;

  if (style.isBitSet(nn::hid::NpadStyleTag::NpadStyleFullKey)) { index = 0; }
  if (style.isBitSet(nn::hid::NpadStyleTag::NpadStyleHandheld)) { index = 1; }
  if (style.isBitSet(nn::hid::NpadStyleTag::NpadStyleJoyDual)) { index = 2; }
  if (style.isBitSet(nn::hid::NpadStyleTag::NpadStyleJoyLeft)) { index = 3; }
  if (style.isBitSet(nn::hid::NpadStyleTag::NpadStyleJoyRight)) { index = 4; }
  if (style.isBitSet(nn::hid::NpadStyleTag::NpadStyleGc)) { index = 5; }
  if (style.isBitSet(nn::hid::NpadStyleTag::NpadStylePalma)) { index = 6; }
  if (style.isBitSet(nn::hid::NpadStyleTag::NpadStyleLark)) { index = 7; }
  if (style.isBitSet(nn::hid::NpadStyleTag::NpadStyleHandheldLark)) { index = 8; }
  if (style.isBitSet(nn::hid::NpadStyleTag::NpadStyleLucia)) { index = 9; }
  if (style.isBitSet(nn::hid::NpadStyleTag::NpadStyleLagon)) { index = 10; }
  if (style.isBitSet(nn::hid::NpadStyleTag::NpadStyleLager)) { index = 11; }
  if (style.isBitSet(nn::hid::NpadStyleTag::NpadStyleSystemExt)) { index = 12; }
  if (style.isBitSet(nn::hid::NpadStyleTag::NpadStyleSystem)) { index = 13; }

  if (index != -1u) {
    return styleNames[index];
  } else {
    return "Unknown";
  }
}

void InputHelper::initKBM() {
  nn::hid::InitializeKeyboard();
  nn::hid::InitializeMouse();
}

void InputHelper::updatePadState() {
  prevControllerState = curControllerState;
  tryGetContState(&curControllerState, selectedPort);

  prevKeyboardState = curKeyboardState;
  nn::hid::GetKeyboardState(&curKeyboardState);

  prevMouseState = curMouseState;
  nn::hid::GetMouseState(&curMouseState);

  prevTouchState = curTouchState;
  nn::hid::GetTouchScreenState(&curTouchState);

//  if (isHoldZR() && isHoldR() && isPressPadUp()) {
//    toggleInput = !toggleInput;
//  }
  if ((isHoldLeftStick() && isPressRightStick()) || (isHoldRightStick() && isPressLeftStick())) {
    toggleInput = !toggleInput;
  }
}

bool InputHelper::tryGetContState(nn::hid::NpadBaseState *state, ulong port) {

  nn::hid::NpadStyleSet styleSet = nn::hid::GetNpadStyleSet(port);
  isReadInput = true;
  bool result = true;

  nn::hid::GetNpadState((nn::hid::NpadFullKeyState *) state, port);

  if (styleSet.isBitSet(nn::hid::NpadStyleTag::NpadStyleFullKey)) {
    nn::hid::GetNpadState((nn::hid::NpadFullKeyState *) state, port);
  } else if (styleSet.isBitSet(nn::hid::NpadStyleTag::NpadStyleHandheld)) {
    nn::hid::GetNpadState((nn::hid::NpadHandheldState *) state, port);
  } else if (styleSet.isBitSet(nn::hid::NpadStyleTag::NpadStyleJoyDual)) {
    nn::hid::GetNpadState((nn::hid::NpadJoyDualState *) state, port);
  } else {
    result = false;
  }

  isReadInput = false;

  return result;

}

bool InputHelper::isButtonHold(nn::hid::NpadButton button) {
  return curControllerState.mButtons.isBitSet(button);
}

bool InputHelper::isButtonPress(nn::hid::NpadButton button) {
  return curControllerState.mButtons.isBitSet(button) && !prevControllerState.mButtons.isBitSet(button);
}

bool InputHelper::isButtonRelease(nn::hid::NpadButton button) {
  return !curControllerState.mButtons.isBitSet(button) && prevControllerState.mButtons.isBitSet(button);
}

bool InputHelper::isKeyHold(nn::hid::KeyboardKey key) {
  return curKeyboardState.keys.isBitSet(key);
}

bool InputHelper::isKeyPress(nn::hid::KeyboardKey key) {
  return curKeyboardState.keys.isBitSet(key) && !prevKeyboardState.keys.isBitSet(key);
}

bool InputHelper::isKeyRelease(nn::hid::KeyboardKey key) {
  return !curKeyboardState.keys.isBitSet(key) && prevKeyboardState.keys.isBitSet(key);
}

bool InputHelper::isModifierActive(nn::hid::KeyboardModifier modifier) {
  return curKeyboardState.modifiers.isBitSet(modifier);
}

bool InputHelper::isMouseHold(nn::hid::MouseButton button) {
  return curMouseState.buttons.isBitSet(button);
}

bool InputHelper::isMousePress(nn::hid::MouseButton button) {
  return curMouseState.buttons.isBitSet(button) && !prevMouseState.buttons.isBitSet(button);
}

bool InputHelper::isMouseRelease(nn::hid::MouseButton button) {
  return !curMouseState.buttons.isBitSet(button) && prevMouseState.buttons.isBitSet(button);
}

bool InputHelper::isMouseConnected() {
  return curMouseState.attributes.isBitSet(nn::hid::MouseAttribute::IsConnected);
}

void InputHelper::getMouseCoords(float *x, float *y) {
  *x = curMouseState.x;
  *y = curMouseState.y;
}

void InputHelper::getScrollDelta(float *x, float *y) {
  *x = curMouseState.wheelDeltaX;
  *y = curMouseState.wheelDeltaY;
}

bool InputHelper::getTouchCoords(s32 *x, s32 *y) {
  if (curTouchState.count > 0) {
    *x = curTouchState.touches[0].X;
    *y = curTouchState.touches[0].Y;
    return true;
  }
  return false;
}

bool InputHelper::isHoldTouch() {
  return curTouchState.count > 0;
}

bool InputHelper::isPressTouch() {
  return curTouchState.count > 0 && prevTouchState.count == 0;
}

bool InputHelper::isReleaseTouch() {
  return curTouchState.count == 0 && prevTouchState.count > 0;
}