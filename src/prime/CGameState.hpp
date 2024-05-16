#pragma once

#include "CGameInstanceState.h"
#include "CObjectId.hpp"
#include "CWorldStateMP1.h"
#include "program/GetField.hpp"
#include "types.h"

namespace rstl {
  template<typename T>
  class ncrc_ptr;
}

class CUniverseInfo;

class SSaveInfo;

class CInputBitStream;

class COutputBitStream;

class CGameOptions;

class CSystemState;

class SAreaInfoContext;

class CGuid;

class CGameState {
public:
  struct SInitialState {
    SInitialState(CObjectId const&);
    SInitialState(CObjectId const&,SSaveInfo const&);
    SInitialState(CInputStream &,uint);
    void PutTo(COutputStream &);

    CObjectId mInitialId;
    int mSaveSlot;
  };

  enum EReturnTrueIfAreaInvalid {
  };
  enum EForcePreserveData {
  };

  CGameState(rstl::ncrc_ptr<CUniverseInfo> const &, CGameState::SInitialState const &);
  CGameState(CInputBitStream &, rstl::ncrc_ptr<CUniverseInfo> const &, uint, rstl::ncrc_ptr<CGameOptions> const &);
  ~CGameState();

  void InitQuickLoadBuffer(void);
  void InitializeFromStream(CInputBitStream &, uint);
  void ResetGameInstanceState(void);
  void PreSave(CObjectId const &);
  void SetSavePointName(rstl::basic_string<char, rstl::char_traits<char>, rstl::rmemory_allocator> const &);
  void PutTo(COutputBitStream &);
  void StreamInSystemState(CInputStream &, uint);
  void StreamOutSystemState(COutputStream &);
  void CopyOutSystemStateFrontEndSettings(CSystemState &);
  void ConstructAreaStatesIfRequired(SAreaInfoContext const &);
  void SetDesiredAreaId(CObjectId const &);
  void GetSavePointName(void);
  void PlayerState(void);
  void GetPlayerState(void);
  void ComputeGameProgress(void);
  void SetTotalPlayTime(double);
  void GetGameMode4CC(void);
  void IsInFrontEndArea(CGameState::EReturnTrueIfAreaInvalid);
  void WasLoadedFromSaveFile(void);
  void SetSaveSlot(int);
  void IsCinematicSkippable(CGuid const &);
  void SetCinematicSkippable(CGuid const &, bool);
  void SetCinematicForceSkippableOverride(bool);
  void StreamNewGameState(CInputStream &, rstl::ncrc_ptr<CUniverseInfo> const &, uint,
                          rstl::ncrc_ptr<CGameOptions> const &);
  void CreateNewGameState(rstl::ncrc_ptr<CUniverseInfo> const &, CGameState::SInitialState const &);
  void ResetGameState(CGameState const &, CGameState::SInitialState const &, CGameState::EForcePreserveData);
  void SetInitialState(CGameState::SInitialState const &);
  void ResetWithQuickLoadBuffer(void);

  static void ContinueWithNewInitialState(CGameState &, CGameState::SInitialState const &);

  static bool mCinematicForceSkippableOverride;

  inline double &GetPlayTime() {
    return *reinterpret_cast<double*>(reinterpret_cast<size_t>(this) + 0xffb08);
  }
};

class CStateManagerGameLogicMP1;
class CStateManager {
public:
  CGameState& GameState(void);
  CGameState* GetGameState(void) const;

  CGameInstanceState& GameInstanceState(void);
  CGameInstanceState* GetGameInstanceState(void) const;

  inline CStateManagerGameLogicMP1* GameLogic() {
    return GetField<CStateManagerGameLogicMP1>(this, 0x4e0150);
  }
};

class CStateManagerUpdateAccess;
class CSystemStateMP1;

class CPlayerStateMP1;
class CGameStateMP1 {
public:

  ~CGameStateMP1();
  CGameStateMP1(void);
  CGameStateMP1(CInputStream &,uint);
  void ResetFromStream(CInputStream &,uint);
  void InitializeWithUniverseInfo(CGameState const&);
  void SetCurrentWorldId(CObjectId);
  void PutTo(COutputStream &);
  void StreamInSystemState(CInputStream &,uint);
  void StreamOutSystemState(COutputStream &);
  void StreamInStateForGameCompletion(CInputStream &);
  void StreamOutStateForGameCompletion(COutputStream &);
  void CopyInSystemStateFrontEndSettings(CSystemStateMP1 const&);

  CWorldStateMP1* StateForWorld(CObjectId);

  CWorldStateMP1* CurrentWorldState(void);
  const CWorldStateMP1* GetCurrentWorldState(void) const;

  CPlayerStateMP1* PlayerState(void);
  const CPlayerStateMP1* GetPlayerState(void) const;

  void SetGameCompleted(bool);
  void GetDamageMultiplier(void);
  void GetWeaponMultiplier(void);
  void IsIntroLevel(void);

  inline CPlayerStateMP1* GetPlayerState_2() { return GetField<CPlayerStateMP1>(this, 0x20); }
};
extern CGameStateMP1* gpGameState;

class CPlayerMP1;
class CStateManagerGameLogicMP1 {
public:
  enum class EGameState : uint32_t { Running, SoftPaused, Paused };
  void GameMainLoop(CStateManager& mgr, CStateManagerUpdateAccess& mgrUpdAcc, float dt);
  static CPlayerStateMP1* PlayerState();
  CPlayerMP1* PlayerActor();
  void SetGameState(EGameState state);
};