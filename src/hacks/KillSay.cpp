
#include "../xorstring.hpp"

/*
 * KillSay.cpp
 *
 *  Created on: Jan 19, 2017
 *      Author: nullifiedcat
 */

#include "KillSay.h"
#include "../common.h"
#include "../sdk.h"

namespace hacks { namespace shared { namespace killsay {

static CatEnum killsay_enum({XStr("NONE"), XStr("CUSTOM"), XStr("DEFAULT"), XStr("NCC - OFFENSIVE"), XStr("NCC - MLG")});
static CatVar killsay_mode(killsay_enum, XStr("killsay"), XStr("0"), XStr("Killsay"), XStr("Defines source of killsay lines. CUSTOM killsay file must be set in cat_killsay_file and loaded with cat_killsay_reload (Use console!)"));
static CatVar filename(CV_STRING, XStr("killsay_file"), XStr("killsays.txt"), XStr("Killsay file"), XStr("Killsay file name. Should be located in cathook data folder"));
static CatCommand reload(XStr("killsay_reload"), XStr("Reload killsays"), Reload);

const std::string tf_classes_killsay[] = {
	XStr("class"),
	XStr("scout"),
	XStr("sniper"),
	XStr("soldier"),
	XStr("demoman"),
	XStr("medic"),
	XStr("heavy"),
	XStr("pyro"),
	XStr("spy"),
	XStr("engineer")
};

const std::string tf_teams_killsay[] = {
	XStr("RED"),
	XStr("BLU")
};

TextFile file {};

std::string ComposeKillSay(IGameEvent* event) {
	const std::vector<std::string>* source = nullptr;
	switch ((int)killsay_mode) {
	case 1:
		source = &file.lines; break;
	case 2:
		source = &builtin_default; break;
	case 3:
		source = &builtin_nonecore_offensive; break;
	case 4	:
		source = &builtin_nonecore_mlg; break;
	}
	if (!source || source->size() == 0) return XStr("");
	if (!event) return XStr("");
	int vid = event->GetInt(XStr("userid"));
	int kid = event->GetInt(XStr("attacker"));
	if (kid == vid) return XStr("");
	if (g_IEngine->GetPlayerForUserID(kid) != g_IEngine->GetLocalPlayer()) return XStr("");
	std::string msg = source->at(rand() % source->size());
	player_info_s info;
	g_IEngine->GetPlayerInfo(g_IEngine->GetPlayerForUserID(vid), &info);
	ReplaceString(msg, XStr("%name%"), std::string(info.name));
	CachedEntity* ent = ENTITY(g_IEngine->GetPlayerForUserID(vid));
	int clz = g_pPlayerResource->GetClass(ent);
	ReplaceString(msg, XStr("%class%"), tf_classes_killsay[clz]);
	player_info_s infok;
	g_IEngine->GetPlayerInfo(g_IEngine->GetPlayerForUserID(kid), &infok);
	ReplaceString(msg, XStr("%killer%"), std::string(infok.name));
	ReplaceString(msg, XStr("%team%"), tf_teams_killsay[ent->m_iTeam - 2]);
	ReplaceString(msg, XStr("%myteam%"), tf_teams_killsay[LOCAL_E->m_iTeam - 2]);
	ReplaceString(msg, XStr("%myclass%"), tf_classes_killsay[g_pPlayerResource->GetClass(LOCAL_E)]);
	ReplaceString(msg, XStr("\\n"), XStr("\n"));
	return msg;
}

KillSayEventListener& getListener() {
	static KillSayEventListener listener;
	return listener;
}

void Reload() {
	file.Load(std::string(filename.GetString()));
}

void Init() {
	g_IEventManager2->AddListener(&getListener(), (const char*)XStr("player_death"), false);
	filename.InstallChangeCallback([](IConVar* var, const char* pszOV, float flOV) {
		file.TryLoad(std::string(filename.GetString()));
	});
}

void Shutdown() {
	g_IEventManager2->RemoveListener(&getListener());
}

// Thanks HellJustFroze for linking me http://daviseford.com/shittalk/
const std::vector<std::string> builtin_default = {
	XStr("Don't worry guys, I'm a garbage collector. I'm used to carrying trash."),
	XStr("%name% is the human equivalent of a participation award."),
	XStr("I would insult %name%, but nature did a better job."),
	XStr("%name%, perhaps your strategy should include trying."),
	XStr("Some people get paid to suck, you do it for free, %name%."),
	XStr("%name%, I'd tell you to commit suicide, but then you'd have a kill."),
	XStr("You must really like that respawn timer, %name%."),

	XStr("If your main is %class%, you should give up."),
	XStr("Hey %name%, i see you can't play %class%. Try quitting the game.")
	XStr("%team% is filled with spergs"),
	XStr("%name%@gmail.com to vacreview@valvesoftware.com\nFOUND CHEATER"),
	XStr("\n☐ Not rekt\n ☑ Rekt\n ☑ Really Rekt\n ☑ Tyrannosaurus Rekt")
};

const std::vector<std::string> builtin_nonecore_offensive = {
		XStr("%name%, you are noob."), XStr("%name%, do you even lift?"), XStr("%name%, you're a faggot."), XStr("%name%, stop cheating."),
		XStr("%name%: Mom, call the police - I've got headshoted again!"), XStr("Right into your face, %name%."),
		XStr("Into your face, pal."), XStr("Keep crying, baby."), XStr("Faggot. Noob."), XStr("You are dead, not big surprise."),
		XStr("Sit down nerd."), XStr("Fuck you with a rake."), XStr("Eat a man spear, you Jamaican manure salesman."),
		XStr("Wallow in a river of cocks, you pathetic bitch."), XStr("I will go to heaven and you will be in prison."),
		XStr("Piss off, you poor, ignorant, mullet-wearing porch monkey."),
		XStr("Your Mom says your turn-ons consist of butthole licking and scat porn."),
		XStr("Shut up, you'll never be the man your mother is."),
		XStr("It looks like your face caught on fire and someone tried to put it out with a fork."),
		XStr("You're so ugly Hello Kitty said goodbye to you."),
		XStr("Don't you love nature, despite what it did to you?")

};
const std::vector<std::string> builtin_nonecore_mlg = {
		XStr("GET REKT U SCRUB"), XStr("GET REKT M8"), XStr("U GOT NOSCOPED M8"), XStr("U GOT QUICKSCOPED M8"), XStr("2 FAST 4 U, SCRUB"), XStr("U GOT REKT, M8")
};

}}}

void KillSayEventListener::FireGameEvent(IGameEvent* event) {
	if (!hacks::shared::killsay::killsay_mode) return;
	std::string message = hacks::shared::killsay::ComposeKillSay(event);
	if (message.size()) {
		chat_stack::Say(message);
	}
}
