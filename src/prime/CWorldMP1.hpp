#pragma once

#include "program/GetField.hpp"
#include "types.h"

class CWorldMP1 {
public:
  inline int32_t GetLoadPhase() { return *GetField<int32_t>(this, 8); }
};