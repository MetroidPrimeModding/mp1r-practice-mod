#pragma once

#include "prime/rstl/auto_ptr.h"

namespace NGameInstanceState {
    class IPlayerInstanceState;

    enum EQuitReason {
        None = -1,
        kQR_Warp = 7,
    };
}

class CStateManager;

class CGameInstanceState {
public:
    CGameInstanceState(rstl::auto_ptr<NGameInstanceState::IPlayerInstanceState> const&);
    ~CGameInstanceState();

    void GameCameraState(CStateManager const&);
    void GameFlowState(CStateManager const&);
    void HUDState(CStateManager const&);
    void PlayerInstanceStateInterface(CStateManager const&);
    void GraphicalTransitionState(CStateManager const&);
    void AITaskMgr(CStateManager const&);
    void QuitGame(NGameInstanceState::EQuitReason);
    void GetWantsToQuit(void);
    NGameInstanceState::EQuitReason GetQuitReason(void);
    void CheckStateManagerTypeForWriteAccess(CStateManager const&);
};