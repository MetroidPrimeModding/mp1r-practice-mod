#pragma once

enum class EControl : int {

};

class CFinalInput {
public:
  char DATA_HOLDER[0x398];

  char GetDigitalHeld(EControl control) const;
  char GetDigitalPressed(EControl control) const;
  char GetDigitalReleased(EControl control) const;
  float GetAnalog(EControl control) const;
};