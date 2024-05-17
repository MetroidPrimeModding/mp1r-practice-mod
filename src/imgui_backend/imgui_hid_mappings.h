#pragma once

#include "imgui.h"
#include "nn/hid.h"

constexpr int npad_mapping[][2] = {
    {ImGuiKey_GamepadFaceDown,    static_cast<const int>(nn::hid::NpadButton::A)},
    {ImGuiKey_GamepadFaceRight,   static_cast<const int>(nn::hid::NpadButton::B)},
    {ImGuiKey_GamepadFaceUp,      static_cast<const int>(nn::hid::NpadButton::X)},
    {ImGuiKey_GamepadFaceLeft,    static_cast<const int>(nn::hid::NpadButton::Y)},
    {ImGuiKey_GamepadL1,          static_cast<const int>(nn::hid::NpadButton::L)},
    {ImGuiKey_GamepadR1,          static_cast<const int>(nn::hid::NpadButton::R)},
    {ImGuiKey_GamepadL2,          static_cast<const int>(nn::hid::NpadButton::ZL)},
    {ImGuiKey_GamepadR2,          static_cast<const int>(nn::hid::NpadButton::ZR)},
    {ImGuiKey_GamepadStart,       static_cast<const int>(nn::hid::NpadButton::Plus)},
    {ImGuiKey_GamepadBack,        static_cast<const int>(nn::hid::NpadButton::Minus)},
    {ImGuiKey_GamepadDpadLeft,    static_cast<const int>(nn::hid::NpadButton::Left)},
    {ImGuiKey_GamepadDpadRight,   static_cast<const int>(nn::hid::NpadButton::Right)},
    {ImGuiKey_GamepadDpadUp,      static_cast<const int>(nn::hid::NpadButton::Up)},
    {ImGuiKey_GamepadDpadDown,    static_cast<const int>(nn::hid::NpadButton::Down)},
//    {ImGuiKey_GamepadLStickLeft,  static_cast<const int>(nn::hid::NpadButton::StickLLeft)},
//    {ImGuiKey_GamepadLStickRight, static_cast<const int>(nn::hid::NpadButton::StickLRight)},
//    {ImGuiKey_GamepadLStickUp,    static_cast<const int>(nn::hid::NpadButton::StickLUp)},
//    {ImGuiKey_GamepadLStickDown,  static_cast<const int>(nn::hid::NpadButton::StickLDown)},
};

constexpr int mouse_mapping[][2] = {
    {ImGuiMouseButton_Left,   static_cast<const int>(nn::hid::MouseButton::Left)},
    {ImGuiMouseButton_Right,  static_cast<const int>(nn::hid::MouseButton::Right)},
    {ImGuiMouseButton_Middle, static_cast<const int>(nn::hid::MouseButton::Middle)},
};

constexpr int key_mapping[][2] = {
    {ImGuiKey_None,           0},
    {ImGuiKey_Tab,            static_cast<const int>(nn::hid::KeyboardKey::Tab)},
    {ImGuiKey_LeftArrow,      static_cast<const int>(nn::hid::KeyboardKey::LeftArrow)},
    {ImGuiKey_RightArrow,     static_cast<const int>(nn::hid::KeyboardKey::RightArrow)},
    {ImGuiKey_UpArrow,        static_cast<const int>(nn::hid::KeyboardKey::UpArrow)},
    {ImGuiKey_DownArrow,      static_cast<const int>(nn::hid::KeyboardKey::DownArrow)},
    {ImGuiKey_PageUp,         static_cast<const int>(nn::hid::KeyboardKey::PageUp)},
    {ImGuiKey_PageDown,       static_cast<const int>(nn::hid::KeyboardKey::PageDown)},
    {ImGuiKey_Home,           static_cast<const int>(nn::hid::KeyboardKey::Home)},
    {ImGuiKey_End,            static_cast<const int>(nn::hid::KeyboardKey::End)},
    {ImGuiKey_Insert,         static_cast<const int>(nn::hid::KeyboardKey::Insert)},
    {ImGuiKey_Delete,         static_cast<const int>(nn::hid::KeyboardKey::Delete)},
    {ImGuiKey_Backspace,      static_cast<const int>(nn::hid::KeyboardKey::Backspace)},
    {ImGuiKey_Space,          static_cast<const int>(nn::hid::KeyboardKey::Space)},
    {ImGuiKey_Enter,          static_cast<const int>(nn::hid::KeyboardKey::Return)},
    {ImGuiKey_Escape,         static_cast<const int>(nn::hid::KeyboardKey::Escape)},
    {ImGuiKey_LeftCtrl,       static_cast<const int>(nn::hid::KeyboardKey::LeftControl)},
    {ImGuiKey_LeftShift,      static_cast<const int>(nn::hid::KeyboardKey::LeftShift)},
    {ImGuiKey_LeftAlt,        static_cast<const int>(nn::hid::KeyboardKey::LeftAlt)},
    {ImGuiKey_LeftSuper,      static_cast<const int>(nn::hid::KeyboardKey::LeftGui)},
    {ImGuiKey_RightCtrl,      static_cast<const int>(nn::hid::KeyboardKey::RightControl)},
    {ImGuiKey_RightShift,     static_cast<const int>(nn::hid::KeyboardKey::RightShift)},
    {ImGuiKey_RightAlt,       static_cast<const int>(nn::hid::KeyboardKey::RightAlt)},
    {ImGuiKey_RightSuper,     static_cast<const int>(nn::hid::KeyboardKey::RightGui)},
    {ImGuiKey_Menu,           static_cast<const int>(nn::hid::KeyboardKey::Application)}, // not positive about this
    {ImGuiKey_0,              static_cast<const int>(nn::hid::KeyboardKey::D0)},
    {ImGuiKey_1,              static_cast<const int>(nn::hid::KeyboardKey::D1)},
    {ImGuiKey_2,              static_cast<const int>(nn::hid::KeyboardKey::D2)},
    {ImGuiKey_3,              static_cast<const int>(nn::hid::KeyboardKey::D3)},
    {ImGuiKey_4,              static_cast<const int>(nn::hid::KeyboardKey::D4)},
    {ImGuiKey_5,              static_cast<const int>(nn::hid::KeyboardKey::D5)},
    {ImGuiKey_6,              static_cast<const int>(nn::hid::KeyboardKey::D6)},
    {ImGuiKey_7,              static_cast<const int>(nn::hid::KeyboardKey::D7)},
    {ImGuiKey_8,              static_cast<const int>(nn::hid::KeyboardKey::D8)},
    {ImGuiKey_9,              static_cast<const int>(nn::hid::KeyboardKey::D9)},
    {ImGuiKey_A,              static_cast<const int>(nn::hid::KeyboardKey::A)},
    {ImGuiKey_B,              static_cast<const int>(nn::hid::KeyboardKey::B)},
    {ImGuiKey_C,              static_cast<const int>(nn::hid::KeyboardKey::C)},
    {ImGuiKey_D,              static_cast<const int>(nn::hid::KeyboardKey::D)},
    {ImGuiKey_E,              static_cast<const int>(nn::hid::KeyboardKey::E)},
    {ImGuiKey_F,              static_cast<const int>(nn::hid::KeyboardKey::F)},
    {ImGuiKey_G,              static_cast<const int>(nn::hid::KeyboardKey::G)},
    {ImGuiKey_H,              static_cast<const int>(nn::hid::KeyboardKey::H)},
    {ImGuiKey_I,              static_cast<const int>(nn::hid::KeyboardKey::I)},
    {ImGuiKey_J,              static_cast<const int>(nn::hid::KeyboardKey::J)},
    {ImGuiKey_K,              static_cast<const int>(nn::hid::KeyboardKey::K)},
    {ImGuiKey_L,              static_cast<const int>(nn::hid::KeyboardKey::L)},
    {ImGuiKey_M,              static_cast<const int>(nn::hid::KeyboardKey::M)},
    {ImGuiKey_N,              static_cast<const int>(nn::hid::KeyboardKey::N)},
    {ImGuiKey_O,              static_cast<const int>(nn::hid::KeyboardKey::O)},
    {ImGuiKey_P,              static_cast<const int>(nn::hid::KeyboardKey::P)},
    {ImGuiKey_Q,              static_cast<const int>(nn::hid::KeyboardKey::Q)},
    {ImGuiKey_R,              static_cast<const int>(nn::hid::KeyboardKey::R)},
    {ImGuiKey_S,              static_cast<const int>(nn::hid::KeyboardKey::S)},
    {ImGuiKey_T,              static_cast<const int>(nn::hid::KeyboardKey::T)},
    {ImGuiKey_U,              static_cast<const int>(nn::hid::KeyboardKey::U)},
    {ImGuiKey_V,              static_cast<const int>(nn::hid::KeyboardKey::V)},
    {ImGuiKey_W,              static_cast<const int>(nn::hid::KeyboardKey::W)},
    {ImGuiKey_X,              static_cast<const int>(nn::hid::KeyboardKey::X)},
    {ImGuiKey_Y,              static_cast<const int>(nn::hid::KeyboardKey::Y)},
    {ImGuiKey_Z,              static_cast<const int>(nn::hid::KeyboardKey::Z)},
    {ImGuiKey_F1,             static_cast<const int>(nn::hid::KeyboardKey::F1)},
    {ImGuiKey_F2,             static_cast<const int>(nn::hid::KeyboardKey::F2)},
    {ImGuiKey_F3,             static_cast<const int>(nn::hid::KeyboardKey::F3)},
    {ImGuiKey_F4,             static_cast<const int>(nn::hid::KeyboardKey::F4)},
    {ImGuiKey_F5,             static_cast<const int>(nn::hid::KeyboardKey::F5)},
    {ImGuiKey_F6,             static_cast<const int>(nn::hid::KeyboardKey::F6)},
    {ImGuiKey_F7,             static_cast<const int>(nn::hid::KeyboardKey::F7)},
    {ImGuiKey_F8,             static_cast<const int>(nn::hid::KeyboardKey::F8)},
    {ImGuiKey_F9,             static_cast<const int>(nn::hid::KeyboardKey::F9)},
    {ImGuiKey_F10,            static_cast<const int>(nn::hid::KeyboardKey::F10)},
    {ImGuiKey_F11,            static_cast<const int>(nn::hid::KeyboardKey::F11)},
    {ImGuiKey_F12,            static_cast<const int>(nn::hid::KeyboardKey::F12)},
    {ImGuiKey_Apostrophe,     static_cast<const int>(nn::hid::KeyboardKey::Quote)},
    {ImGuiKey_Comma,          static_cast<const int>(nn::hid::KeyboardKey::Comma)},
    {ImGuiKey_Minus,          static_cast<const int>(nn::hid::KeyboardKey::Minus)},
    {ImGuiKey_Period,         static_cast<const int>(nn::hid::KeyboardKey::Period)},
    {ImGuiKey_Slash,          static_cast<const int>(nn::hid::KeyboardKey::Slash)},
    {ImGuiKey_Semicolon,      static_cast<const int>(nn::hid::KeyboardKey::Semicolon)},
    {ImGuiKey_Equal,          static_cast<const int>(nn::hid::KeyboardKey::Plus)},
    {ImGuiKey_LeftBracket,    static_cast<const int>(nn::hid::KeyboardKey::OpenBracket)},
    {ImGuiKey_Backslash,      static_cast<const int>(nn::hid::KeyboardKey::Backslash)},
    {ImGuiKey_RightBracket,   static_cast<const int>(nn::hid::KeyboardKey::CloseBracket)},
    {ImGuiKey_GraveAccent,    static_cast<const int>(nn::hid::KeyboardKey::Backquote)},
    {ImGuiKey_CapsLock,       static_cast<const int>(nn::hid::KeyboardKey::CapsLock)},
    {ImGuiKey_ScrollLock,     static_cast<const int>(nn::hid::KeyboardKey::ScrollLock)},
    {ImGuiKey_NumLock,        static_cast<const int>(nn::hid::KeyboardKey::NumLock)},
    {ImGuiKey_PrintScreen,    static_cast<const int>(nn::hid::KeyboardKey::PrintScreen)},
    {ImGuiKey_Pause,          static_cast<const int>(nn::hid::KeyboardKey::Pause)},
    {ImGuiKey_Keypad0,        static_cast<const int>(nn::hid::KeyboardKey::NumPad0)},
    {ImGuiKey_Keypad1,        static_cast<const int>(nn::hid::KeyboardKey::NumPad1)},
    {ImGuiKey_Keypad2,        static_cast<const int>(nn::hid::KeyboardKey::NumPad2)},
    {ImGuiKey_Keypad3,        static_cast<const int>(nn::hid::KeyboardKey::NumPad3)},
    {ImGuiKey_Keypad4,        static_cast<const int>(nn::hid::KeyboardKey::NumPad4)},
    {ImGuiKey_Keypad5,        static_cast<const int>(nn::hid::KeyboardKey::NumPad5)},
    {ImGuiKey_Keypad6,        static_cast<const int>(nn::hid::KeyboardKey::NumPad6)},
    {ImGuiKey_Keypad7,        static_cast<const int>(nn::hid::KeyboardKey::NumPad7)},
    {ImGuiKey_Keypad8,        static_cast<const int>(nn::hid::KeyboardKey::NumPad8)},
    {ImGuiKey_Keypad9,        static_cast<const int>(nn::hid::KeyboardKey::NumPad9)},
    {ImGuiKey_KeypadDecimal,  static_cast<const int>(nn::hid::KeyboardKey::NumPadDot)},
    {ImGuiKey_KeypadDivide,   static_cast<const int>(nn::hid::KeyboardKey::NumPadDivide)},
    {ImGuiKey_KeypadMultiply, static_cast<const int>(nn::hid::KeyboardKey::NumPadMultiply)},
    {ImGuiKey_KeypadSubtract, static_cast<const int>(nn::hid::KeyboardKey::NumPadSubtract)},
    {ImGuiKey_KeypadAdd,      static_cast<const int>(nn::hid::KeyboardKey::NumPadAdd)},
    {ImGuiKey_KeypadEnter,    static_cast<const int>(nn::hid::KeyboardKey::NumPadEnter)},
    {ImGuiKey_KeypadEqual,    static_cast<const int>(nn::hid::KeyboardKey::NumPadEquals)},
};

constexpr char getKeyCode(ImGuiKey key, bool isAltCode, bool isNumLock) {
  isAltCode = !isAltCode; // i dont want to rewrite this sorry
  switch (key) {
  case ImGuiKey_Space:
    return ' ';
  case ImGuiKey_0:
    return isAltCode ? '0' : ')';
  case ImGuiKey_1:
    return isAltCode ? '1' : '!';
  case ImGuiKey_2:
    return isAltCode ? '2' : '@';
  case ImGuiKey_3:
    return isAltCode ? '3' : '#';
  case ImGuiKey_4:
    return isAltCode ? '4' : '$';
  case ImGuiKey_5:
    return isAltCode ? '5' : '%';
  case ImGuiKey_6:
    return isAltCode ? '6' : '^';
  case ImGuiKey_7:
    return isAltCode ? '7' : '&';
  case ImGuiKey_8:
    return isAltCode ? '8' : '*';
  case ImGuiKey_9:
    return isAltCode ? '9' : '(';
  case ImGuiKey_Keypad0:
    return isNumLock ? '0' : 0;
  case ImGuiKey_Keypad1:
    return isNumLock ? '1' : 0;
  case ImGuiKey_Keypad2:
    return isNumLock ? '2' : 0;
  case ImGuiKey_Keypad3:
    return isNumLock ? '3' : 0;
  case ImGuiKey_Keypad4:
    return isNumLock ? '4' : 0;
  case ImGuiKey_Keypad5:
    return isNumLock ? '5' : 0;
  case ImGuiKey_Keypad6:
    return isNumLock ? '6' : 0;
  case ImGuiKey_Keypad7:
    return isNumLock ? '7' : 0;
  case ImGuiKey_Keypad8:
    return isNumLock ? '8' : 0;
  case ImGuiKey_Keypad9:
    return isNumLock ? '9' : 0;
  case ImGuiKey_KeypadDecimal:
    return isNumLock ? '.' : 0;
  case ImGuiKey_KeypadDivide:
    return isNumLock ? '/' : 0;
  case ImGuiKey_KeypadMultiply:
    return isNumLock ? '*' : 0;
  case ImGuiKey_KeypadSubtract:
    return isNumLock ? '-' : 0;
  case ImGuiKey_KeypadAdd:
    return isNumLock ? '+' : 0;
  case ImGuiKey_KeypadEqual:
    return isNumLock ? '=' : 0;
  case ImGuiKey_A:
    return isAltCode ? 'a' : 'A';
  case ImGuiKey_B:
    return isAltCode ? 'b' : 'B';
  case ImGuiKey_C:
    return isAltCode ? 'c' : 'C';
  case ImGuiKey_D:
    return isAltCode ? 'd' : 'D';
  case ImGuiKey_E:
    return isAltCode ? 'e' : 'E';
  case ImGuiKey_F:
    return isAltCode ? 'f' : 'F';
  case ImGuiKey_G:
    return isAltCode ? 'g' : 'G';
  case ImGuiKey_H:
    return isAltCode ? 'h' : 'H';
  case ImGuiKey_I:
    return isAltCode ? 'i' : 'I';
  case ImGuiKey_J:
    return isAltCode ? 'j' : 'J';
  case ImGuiKey_K:
    return isAltCode ? 'k' : 'K';
  case ImGuiKey_L:
    return isAltCode ? 'l' : 'L';
  case ImGuiKey_M:
    return isAltCode ? 'm' : 'M';
  case ImGuiKey_N:
    return isAltCode ? 'n' : 'N';
  case ImGuiKey_O:
    return isAltCode ? 'o' : 'O';
  case ImGuiKey_P:
    return isAltCode ? 'p' : 'P';
  case ImGuiKey_Q:
    return isAltCode ? 'q' : 'Q';
  case ImGuiKey_R:
    return isAltCode ? 'r' : 'R';
  case ImGuiKey_S:
    return isAltCode ? 's' : 'S';
  case ImGuiKey_T:
    return isAltCode ? 't' : 'T';
  case ImGuiKey_U:
    return isAltCode ? 'u' : 'U';
  case ImGuiKey_V:
    return isAltCode ? 'v' : 'V';
  case ImGuiKey_W:
    return isAltCode ? 'w' : 'W';
  case ImGuiKey_X:
    return isAltCode ? 'x' : 'X';
  case ImGuiKey_Y:
    return isAltCode ? 'y' : 'Y';
  case ImGuiKey_Z:
    return isAltCode ? 'z' : 'Z';
  case ImGuiKey_Apostrophe:
    return isAltCode ? '\'' : '"';
  case ImGuiKey_Comma:
    return isAltCode ? ',' : '<';
  case ImGuiKey_Minus:
    return isAltCode ? '-' : '_';
  case ImGuiKey_Period:
    return isAltCode ? '.' : '>';
  case ImGuiKey_Slash:
    return isAltCode ? '/' : '?';
  case ImGuiKey_Semicolon:
    return isAltCode ? ';' : ':';
  case ImGuiKey_Equal:
    return isAltCode ? '=' : '+';
  case ImGuiKey_LeftBracket:
    return isAltCode ? '[' : '{';
  case ImGuiKey_Backslash:
    return isAltCode ? '\\' : '|';
  case ImGuiKey_RightBracket:
    return isAltCode ? ']' : '}';
  case ImGuiKey_GraveAccent:
    return isAltCode ? '`' : '~';
  default:
    return 0;
  }
}