/*
 * Misc.h
 *
 *  Created on: Nov 5, 2016
 *      Author: nullifiedcat
 */

#pragma once

#include "common.hpp"
#include "config.h"

namespace hacks
{
namespace shared
{
namespace misc
{

void CreateMove();
#if ENABLE_VISUALS
void DrawText();
#endif

extern int last_number;

extern float last_bucket;
}
}
}

/*class Misc : public IHack {
public:
    Misc();

    virtual void ProcessUserCmd(CUserCmd*) override;
    virtual void Draw() override;

    CatVar* v_bDebugInfo;
    ConCommand* c_Name;
    ConVar* v_bInfoSpam;
    ConVar* v_bFastCrouch;
    CatVar* v_bFlashlightSpam;
    CatVar* v_bMinigunJump; // TF2C
    CatVar* v_bDebugCrits; // TF2C
    CatVar* v_bAntiAFK;
    CatVar* v_bHookInspect;
    CatVar* v_iFakeLag;
    CatVar* v_bCritHack;
    CatVar* v_bTauntSlide;
    CatVar* v_bSuppressCrits;
    //ConVar* v_bDumpEventInfo;
    ConCommand* c_SaveSettings;
    ConCommand* c_Unrestricted;
    ConCommand* c_DumpItemAttributes;
    ConCommand* c_SayLine;
    ConCommand* c_Shutdown;
    ConCommand* c_AddFriend;
    ConCommand* c_AddRage;
    ConCommand* c_DumpVars;
    ConCommand* c_DumpPlayers;
    ConCommand* c_Teamname;
    ConCommand* c_Lockee;
    ConCommand* c_Info;
    ConCommand* c_DumpConds;
    ConCommand* c_Reset;
    ConCommand* c_Disconnect;
    ConCommand* c_Schema;
    ConCommand* c_DisconnectVAC;

    CatVar* v_bCleanChat;
};

void Schema_Reload();
void CC_Misc_Disconnect_VAC();

DECLARE_HACK_SINGLETON(Misc);*/
