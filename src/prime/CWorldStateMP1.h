#pragma once

#include <prime/CObjectId.hpp>
#include <prime/rstl/rc_ptr.hpp>

class CScriptMailboxMP1;
class CMapWorldInfoMP1;

class CWorldStateMP1 {
public:
    CWorldStateMP1(CObjectId);
    CWorldStateMP1(CInputStream &,uint);

    void PutTo(COutputStream &);
    void GetWorldAssetId(void);
    void Mailbox(void);
    void MapWorldInfo(void);
    rstl::rc_ptr<CScriptMailboxMP1>* GetMailbox(void) const;
    rstl::rc_ptr<CMapWorldInfoMP1>* GetMapWorldInfo(void) const;
    CObjectId GetCurrentArea(void) const;
    void SetCurrentArea(CObjectId);
    CObjectId GetDesiredAreaAssetId(void) const;
    void SetDesiredAreaAssetId(CObjectId);

    CObjectId mWorldId;
    CObjectId mAreaId;
    rstl::rc_ptr<CScriptMailboxMP1> mCScriptMailboxMP1;
    rstl::rc_ptr<CMapWorldInfoMP1> mCMapWorldInfoMP1;
    CObjectId mDesiredAreaId;
};