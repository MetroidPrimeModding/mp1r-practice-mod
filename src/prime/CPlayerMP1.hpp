#pragma once

#include "prime/Math.hpp"

class CActorMP1 {
public:
  CTransform4f &GetTransform() const;
  CTransform4f SetTransform(const CTransform4f &transform);
};

class CPhysicsActorMP1: public CActorMP1 {
public:
  CVector3f GetVelocityWR(const CStateManager &mgr) const;
  //_ZN16CPhysicsActorMP113SetVelocityWRER13CStateManagerRK9CVector3f
  //_ZN16CPhysicsActorMP113SetVelocityWRERK13CStateManagerRK9CVector3f
  void SetVelocityWR(CStateManager &mgr, const CVector3f &vel);

  CAxisAngle GetAngularVelocityWR(const CStateManager &mgr) const;
  void SetAngularVelocityWR(CStateManager &mgr, const CAxisAngle &vel);
};

class CPlayerMP1: public CPhysicsActorMP1 {
public:
};