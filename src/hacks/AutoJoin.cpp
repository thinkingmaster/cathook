
#include "../xorstring.hpp"

/*
 * AutoJoin.cpp
 *
 *  Created on: Jul 28, 2017
 *      Author: nullifiedcat
 */

#include "../common.h"
#include "../hack.h"
#include "../copypasted/CSignature.h"

namespace hacks { namespace shared { namespace autojoin {

/*
 * Credits to Blackfire for helping me with auto-requeue!
 */

CatEnum classes_enum({ XStr("DISABLED"), XStr("SCOUT"), XStr("SNIPER"), XStr("SOLDIER"), XStr("DEMOMAN"), XStr("MEDIC"), XStr("HEAVY"), XStr("PYRO"), XStr("SPY"), XStr("ENGINEER") });
CatVar autojoin_team(CV_SWITCH, XStr("autojoin_team"), XStr("0"), XStr("AutoJoin"), XStr("Automatically joins a team"));
CatVar preferred_class(classes_enum, XStr("autojoin_class"), XStr("0"), XStr("AutoJoin class"), XStr("You will pick a class automatically"));

CatVar auto_queue(CV_SWITCH, XStr("autoqueue"), XStr("0"), XStr("AutoQueue"), XStr("Automatically queue in casual matches"));

const std::string classnames[] = {
	XStr("scout"), XStr("sniper"), XStr("soldier"), XStr("demoman"), XStr("medic"), XStr("heavyweapons"), XStr("pyro"), XStr("spy"), XStr("engineer")
};

CatCommand debug_startsearch(XStr("debug_startsearch"), XStr("DEBUG StartSearch"), []() {
	logging::Info(XStr("%d"), g_TFGCClientSystem->RequestSelectWizardStep(4));
});
CatCommand debug_casual(XStr("debug_casual"), XStr("DEBUG Casual"), []() {
	g_IEngine->ExecuteClientCmd(XStr("OpenMatchmakingLobby casual"));
	g_TFGCClientSystem->LoadSearchCriteria();
	//logging::Info(XStr("%d"), g_TFGCClientSystem->RequestSelectWizardStep(6));
});

CatCommand debug_readytosearch(XStr("debug_gcstate"), XStr("DEBUG GCState"), []() {
	logging::Info(XStr("%d"), g_TFGCClientSystem->GetState());
});
CatCommand debug_abandon(XStr("debug_abandon"), XStr("DEBUG Abandon"), []() {
	g_TFGCClientSystem->SendExitMatchmaking(true);
});
bool UnassignedTeam() {
	return !g_pLocalPlayer->team or (g_pLocalPlayer->team == TEAM_SPEC);
}

bool UnassignedClass() {
	return g_pLocalPlayer->clazz != int(preferred_class);
}

void UpdateSearch() {
	if (!auto_queue) return;
	if (g_IEngine->IsInGame()) return;
	static auto last_check = std::chrono::system_clock::now();
	auto s = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - last_check).count();
	if (s < 4) return;

	if (g_TFGCClientSystem->GetState() == 6) {
		logging::Info(XStr("Sending MM request"));
		g_TFGCClientSystem->RequestSelectWizardStep(4);
	} else if (g_TFGCClientSystem->GetState() == 5) {
		g_IEngine->ExecuteClientCmd(XStr("OpenMatchmakingLobby casual"));
		g_TFGCClientSystem->LoadSearchCriteria();
		//logging::Info(XStr("%d"), g_TFGCClientSystem->RequestSelectWizardStep(6));
	}

	last_check = std::chrono::system_clock::now();
}

void Update() {
	static auto last_check = std::chrono::system_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - last_check).count();

	if (ms < 500) {
		return;
	}

	if (autojoin_team and UnassignedTeam()) {
		hack::ExecuteCommand(XStr("jointeam auto"));
	} else if (preferred_class and UnassignedClass()) {
		if (int(preferred_class) < 10)
		g_IEngine->ExecuteClientCmd(format(XStr("join_class "), classnames[int(preferred_class) - 1]).c_str());
	}

	last_check = std::chrono::system_clock::now();
}

}}}
