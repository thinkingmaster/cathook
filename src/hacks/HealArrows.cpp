
#include "../xorstring.hpp"

/*
 * HealArrows.cpp
 *
 *  Created on: Jun 1, 2017
 *      Author: nullifiedcat
 */

#include "../common.h"

namespace hacks { namespace tf2 { namespace healarrow {

static CatVar healarrow_charge(CV_FLOAT, XStr("healarrow_charge"), XStr("0.25"), XStr("Healarrow Charge"));
static CatVar healarrow_timeout(CV_FLOAT, XStr("healarrow_timeout"), XStr("2"), XStr("Healarrow Timeout"));
static CatVar healarrow(CV_SWITCH, XStr("healarrow"), XStr("0"), XStr("Heal Arrow"));
static CatVar healarrow_callout(CV_SWITCH, XStr("healarrow_callout"), XStr("0"), XStr("Call Out"), XStr("Send a message to chat when you heal someone with an arrow"));
static CatVar healarrow_callout_message(CV_STRING, XStr("healarrow_callout_text"), XStr("Hey %%, I've just healed you for $$ HP! Your health is now ##."), XStr("Call Out Text"), XStr("Formatting:\n%% - player name\n$$ - healing amount\n## - new health\n@@ - old health"));

float healarrow_time = 0.0f;

class HealArrowListener : public IGameEventListener {
public:
	virtual void FireGameEvent(KeyValues* event) {
		if (!healarrow) return;
		if (!healarrow_callout) return;
		if (CE_BAD(LOCAL_W)) return;
		if (LOCAL_W->m_iClassID != CL_CLASS(CTFCompoundBow)) return;
		std::string name(event->GetName());
		if (name == XStr("player_hurt")) {
			int attacker = event->GetInt(XStr("attacker"));
			int victim = event->GetInt(XStr("userid"));
			int eid = g_IEngine->GetPlayerForUserID(attacker);
			int vid = g_IEngine->GetPlayerForUserID(victim);
			if (eid == g_IEngine->GetLocalPlayer()) {
				int damageamount = event->GetInt(XStr("damageamount"));
				if (damageamount < 0) {
					player_info_s pinfo;
					if (g_IEngine->GetPlayerInfo(vid, &pinfo)) {
						std::string msg(healarrow_callout_message.GetString());
						ReplaceString(msg, XStr("$$"), std::to_string(-damageamount));
						int health = event->GetInt(XStr("health"));
						ReplaceString(msg, XStr("##"), std::to_string(health));
						ReplaceString(msg, XStr("@@"), std::to_string(health + damageamount));
						ReplaceString(msg, XStr("%%"), pinfo.name);
						chat_stack::Say(msg);
					}
				}
			}
		}
	}
};

HealArrowListener listener;

void Init() {
	g_IGameEventManager->AddListener(&listener, false);
}

void CreateMove() {
	if (CE_BAD(LOCAL_W)) return;
	if (healarrow) {
		if (g_pLocalPlayer->weapon()->m_iClassID == CL_CLASS(CTFCompoundBow)) {
			if (healarrow_time > g_GlobalVars->curtime) healarrow_time = 0.0f;
			float begincharge = CE_FLOAT(g_pLocalPlayer->weapon(), netvar.flChargeBeginTime);
			float charge = 0;
			if (begincharge != 0) {
				charge = g_GlobalVars->curtime - begincharge;
				if (charge > 1.0f) charge = 1.0f;
			}
			if (g_pUserCmd->command_number && ((g_GlobalVars->curtime - healarrow_time) > float(healarrow_timeout)) && (charge > (float)healarrow_charge) && (g_pUserCmd->buttons & IN_ATTACK)) {
				command_number_mod[g_pUserCmd->command_number] = g_pUserCmd->command_number + 450;
				g_pUserCmd->buttons &= ~IN_ATTACK;
				healarrow_time = g_GlobalVars->curtime;
				logging::Info(XStr("healarrow"));
			}
		}
	}
}

void Draw() {
#ifndef TEXTMODE
	if (healarrow) {
		if ((g_GlobalVars->curtime - healarrow_time) < float(healarrow_timeout)) {
			AddCenterString(format(XStr("Heal arrow charge: "), int(min(100.0f, (g_GlobalVars->curtime - healarrow_time) / float(healarrow_timeout)) * 100.0f), '%'), colors::yellow);
//			AddCenterString(format(XStr("Heal arrow time: "), healarrow_time));
		} else {
			AddCenterString(XStr("Heal arrow ready"), colors::green);
		}
	}
#else
	logging::Info(XStr("[WTF] THIS SHOULD NEVER BE CALLED!!! CALL THE POLICE!!!"));
#endif
}

}}}

