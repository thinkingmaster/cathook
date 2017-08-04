
#include "xorstring.hpp"

/*
 * drawmgr.cpp
 *
 *  Created on: May 22, 2017
 *      Author: nullifiedcat
 */

#include "common.h"
#include "hack.h"

void BeginCheatVisuals() {
	std::lock_guard<std::mutex> draw_lock(drawing_mutex);
	if (drawgl::ready_state) {
		FTGL_NewFrame();
		drawgl::Refresh();
		ResetStrings();
	}
}

std::mutex drawing_mutex;

CatVar info_text(CV_SWITCH, XStr("info"), XStr("1"), XStr("Show info"), XStr("Show cathook version in top left corner"));

void DrawCheatVisuals() {
	std::lock_guard<std::mutex> draw_lock(drawing_mutex);
	if (drawgl::ready_state) {
		{
			PROF_SECTION(DRAW_misc);
			SAFE_CALL(hacks::shared::misc::DrawText());
		}
		if (info_text) {
			PROF_SECTION(DRAW_info);
			std::string name_s, reason_s;
			PROF_SECTION(PT_info_text);
			AddSideString(XStr("cathook by nullifiedcat"), colors::RainbowCurrent());
			AddSideString(hack::GetVersion(), GUIColor()); // github commit and date
			AddSideString(hack::GetType(), GUIColor()); //  Compile type
#if ENABLE_GUI
			AddSideString(XStr("Press 'INSERT' or 'F11' key to open/close cheat menu."), GUIColor());
			AddSideString(XStr("Use mouse to navigate in menu."), GUIColor());
#endif
			if (!g_IEngine->IsInGame()
#if ENABLE_GUI
				|| g_pGUI->Visible()
#endif
			) {
				name_s = force_name.GetString();
				if (name_s.length() < 3) name_s = XStr("*Not Set*");
				reason_s = disconnect_reason.GetString();
				if (reason_s.length() < 3) reason_s = XStr("*Not Set*");
				AddSideString(XStr("")); // foolish
				AddSideString(format(XStr("Custom Name: "), name_s), GUIColor());
				AddSideString(format(XStr("Custom Disconnect Reason: "), reason_s), GUIColor());
			}
		}
		if (CE_GOOD(g_pLocalPlayer->entity) && !g_Settings.bInvalid) {
			PROF_SECTION(PT_total_hacks);
			{
				PROF_SECTION(DRAW_aimbot);
				hacks::shared::aimbot::DrawText();
			}
			IF_GAME(IsTF2()) {
				PROF_SECTION(DRAW_skinchanger);
				SAFE_CALL(hacks::tf2::skinchanger::DrawText());
			}
			IF_GAME(IsTF()) {
				PROF_SECTION(DRAW_radar);
				SAFE_CALL(hacks::tf::radar::Draw());
			}
			IF_GAME(IsTF2()) {
				PROF_SECTION(DRAW_healarrows);
				hacks::tf2::healarrow::Draw();
			}
			{
				PROF_SECTION(DRAW_walkbot);
				hacks::shared::walkbot::Draw();
			}
			IF_GAME(IsTF()) {
				PROF_SECTION(PT_antidisguise);
				SAFE_CALL(hacks::tf2::antidisguise::Draw());
			}
			IF_GAME(IsTF()) {
				PROF_SECTION(PT_spyalert);
				SAFE_CALL(hacks::tf::spyalert::Draw());
			}
#ifdef IPC_ENABLED
			IF_GAME(IsTF()) SAFE_CALL(hacks::shared::followbot::Draw());
#endif
			{
				PROF_SECTION(DRAW_esp);
				hacks::shared::esp::Draw();
			}
			DrawSpinner();
			Prediction_PaintTraverse();
		}
		{
			PROF_SECTION(DRAW_strings);
			DrawStrings();
		}
	}

}

void EndCheatVisuals() {
	if (drawgl::ready_state) {

	}
}
