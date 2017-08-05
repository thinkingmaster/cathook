
#include "../xorstring.hpp"

/*
 * AutoTaunt.cpp
 *
 *  Created on: Jul 27, 2017
 *      Author: nullifiedcat
 */

#include "../common.h"
#include "../init.hpp"
#include "../hack.h"

namespace hacks { namespace tf { namespace autotaunt {

CatVar enabled(CV_SWITCH, XStr("autotaunt"), XStr("0"), XStr("AutoTaunt"), XStr("Automatically taunt after killing an enemy, use with walkbots I guess"));
CatVar chance(CV_FLOAT, XStr("autotaunt_chance"), XStr("8"), XStr("AutoTaunt chance"), XStr("Chance of taunting after kill. 0 to 100."), 0.0f, 100.0f);

class AutoTauntListener : public IGameEventListener2 {
public:
	virtual void FireGameEvent(IGameEvent* event) {
		if (!enabled) {
			return;
		}
		if (g_IEngine->GetPlayerForUserID(event->GetInt(XStr("attacker"))) == g_IEngine->GetLocalPlayer()) {
			if (RandomFloat(0, 100) <= float(chance)) {
				hack::ExecuteCommand(XStr("taunt"));
			}
		}
	}
};

AutoTauntListener listener;

// TODO remove event listener when uninjecting?
InitRoutine init([]() {
	g_IEventManager2->AddListener(&listener, XStr("player_death"), false);
});

}}}
