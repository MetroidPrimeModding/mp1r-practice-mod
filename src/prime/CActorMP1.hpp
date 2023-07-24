#pragma once

#include "prime/Math.hpp"

class CActorMP1 {
  public:
  CTransform4f& GetTransform() const;
  CTransform4f SetTransform(const CTransform4f& transform);
};