#pragma once

constexpr int NUM_CONTROLS = 0x38;
enum class EControl : int {
  UNK0 = 0x0,
  UNK1 = 0x1,
  UNK2 = 0x2,
  UNK3 = 0x3,
  UNK4 = 0x4,
  UNK5 = 0x5,
  UNK6 = 0x6,
  UNK7 = 0x7,
  UNK8 = 0x8,
  UNK9 = 0x9,
  UNKA = 0xA,
  UNKB = 0xB,
  UNKC = 0xC,
  UNKD = 0xD,
  UNKE = 0xE,
  LEFT_STICK_Y = 0xF,
  LEFT_STICK_UP = 0x10,
  LEFT_STICK_DOWN = 0x11,
  LEFT_STICK_X = 0x12,
  LEFT_STICK_RIGHT = 0x13,
  LEFT_STICK_LEFT = 0x14,
  RIGHT_STICK_Y = 0x15,
  RIGHT_STICK_UP = 0x16,
  RIGHT_STICK_DOWN = 0x17,
  RIGHT_STICK_X = 0x18,
  RIGHT_STICK_RIGHT = 0x19,
  RIGHT_STICK_LEFT = 0x1A,
  DPAD_UP = 0x1B,
  DPAD_DOWN = 0x1C,
  DPAD_LEFT = 0x1D,
  DPAD_RIGHT = 0x1E,
  A = 0x1F,
  B = 0x20,
  Y = 0x21,
  X = 0x22,
  L = 0x23,
  R = 0x24,
  ZL = 0x25,
  ZR = 0x26,
  MINUS = 0x27,
  PLUS = 0x28,
  UNK29 = 0x29,
  UNK2A = 0x2A,
  LEFT_STICK_CLICK = 0x2B,
  RIGHT_STICK_CLICK = 0x2C,
  UNK2D = 0x2D,
  UNK2E = 0x2E,
  UNK2F = 0x2F,
  UNK30 = 0x30,
  UNK31 = 0x31,
  UNK32 = 0x32,
  UNK33 = 0x33,
  UNK34 = 0x34,
  UNK35 = 0x35,
  UNK36 = 0x36,
  UNK37 = 0x37,
  MAX = 0x38,
};

class CFinalInput {
public:
  char DATA_HOLDER[0x398];

  char GetDigitalHeld(EControl control) const;
  char GetDigitalPressed(EControl control) const;
  char GetDigitalReleased(EControl control) const;
  float GetAnalog(EControl control) const;
};