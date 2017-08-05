
#include "xorstring.hpp"

/*
 * headshake.cpp
 *
 *  Created on: Aug 2, 2017
 *      Author: nullifiedcat
 */

#include "common.h"

#include <queue>

namespace headshake {

/*
 * 		Headshake init
 * 	0: d * 1000 == content length
 * 	1: d => content type
 *
 * 		Headshake data
 * 	2, 3: d * 100000 = 2 bytes of data
 *
 *		Headshake eot
 *	4: d = -dt
 *	5: d * 100000 = command number

const std::vector<float> data_packet_pitch = {
	0.0808, 0.01999, 0.09
};
 */

/*
 * A small packet lasts 2 ticks.
 */
struct smallpacket_s {
	float magic;
	float pitch;
};

/* ACK
 * d * 100000 = entity id
 */

const smallpacket_s ack { 0.02233, 0.04234 };

/* CHU (CatHook User)
 * d * 100000 = entity id
 */

const smallpacket_s chu { 0.00808, 0.01337 };

enum ESmallPackets {
	PK_INV = 0,
	PK_ACK,
	PK_CHU
};

struct headshake_data_s {
	ESmallPackets packet { PK_INV };
	float origin_x { 0.0f };
	float origin_y { 0.0f };
};

headshake_data_s data_array[32] {};

struct headshake_user_data_s {
	bool cathook { false };
	bool acked { false };
	int  treason { 0 };
};

const std::vector<smallpacket_s> packets { ack, chu };

std::unordered_map<unsigned, headshake_user_data_s> userdata {};

struct out_smallpacket_s {
	ESmallPackets type { 0 };
	float data { 0 };
};

std::queue<out_smallpacket_s> queue {};

void UpdateEntity(int id) {
	headshake_data_s& data = data_array[id - 1];
	IClientEntity* ent = g_IEntityList->GetClientEntity(id);
	if (ent && !ent->IsDormant() && !NET_BYTE(ent, netvar.iLifeState)) {
		const Vector& angles = NET_VECTOR(ent, netvar.m_angEyeAngles);
		float pitch = angles.x, yaw = angles.y;
		float dp = pitch - data.origin_x,
			  dy = yaw - data.origin_y;
		if (dp == 0 and dy == 0) return;
		if (data.packet == PK_INV) {
			ESmallPackets packet = PK_INV;
			for (int i = 0; i < packets.size(); i++) {
				if (fabs(dp - packets[i].pitch) < 0.000001) {
					packet = ESmallPackets(i + 1);
				}
			}
			if (packet) {
				if (fabs(dy - packets[packet - 1].magic) < 0.000001) {
					data.packet = packet;
				} else {
					packet = PK_INV;
				}
			}

			if (not packet) {
				data.origin_x = pitch;
				data.origin_y = yaw;
			}
		} else {
			if (fabs(dp - packets[data.packet - 1].pitch) < 0.000001) {
				logging::Info(XStr("[HS] Received packet %d from %d: %.8f"), data.packet, id, dy);
				if (data.packet == PK_CHU) {
					queue.push(out_smallpacket_s { PK_ACK, (float)id * 0.000001 });
				}
			}
			data.packet = PK_INV;
			data.origin_x = pitch;
			data.origin_y = yaw;
		}
	} else {
		data.packet = PK_INV;
	}
}

void Update() {
	for (int i = 1; i < 32 && i < HIGHEST_ENTITY; i++) {
		UpdateEntity(i);
	}
}

struct current_state_s {
	bool sending_packet { false };
	int ticks { 0 };
	out_smallpacket_s packet {};
	float origin_x {};
	float origin_y {};
};

current_state_s state;

CatVar enable_hs(CV_SWITCH, XStr("hs_enable"), XStr("1"), XStr("HeadShake"), XStr("Enable HeadShake system to identify other cathook users"));
static CatVar out_ticks(CV_INT, XStr("hs_out_ticks"), XStr("3"), XStr("Out Ticks"));

static CatCommand send_chu(XStr("hs_chu"), XStr("Debug CHU"), []() {
	queue.push(out_smallpacket_s { PK_CHU, 0.0f });
});

void CreateMove() {
	if (state.sending_packet) {
		if (state.ticks < int(out_ticks)) {
			const auto& p = packets[state.packet.type - 1];
			g_pUserCmd->viewangles.x = state.origin_x + p.pitch;
			g_pUserCmd->viewangles.y = state.origin_y + p.magic;
		} else {
			g_pUserCmd->viewangles.x = state.origin_x;
			g_pUserCmd->viewangles.y = state.origin_y + state.packet.data;
		}
		if (++state.ticks > int(out_ticks) * 2) {
			logging::Info(XStr("[HS] Sent packet: %d %.8f"), state.packet.type, state.packet.data);
			state.ticks = 0;
			state.sending_packet = false;
		}
		return;
	}
	if (queue.empty()) {
		state.origin_x = g_pUserCmd->viewangles.x;
		state.origin_y = g_pUserCmd->viewangles.y;
		state.sending_packet = false;
	} else {
		auto p = queue.front();
		queue.pop();
		state.origin_x = g_pUserCmd->viewangles.x;
		state.origin_y = g_pUserCmd->viewangles.y;
		state.sending_packet = true;
		state.ticks = 0;
		memcpy(&state.packet, &p, sizeof(out_smallpacket_s));
		logging::Info(XStr("[HS] Ready to send packet %d %.8f"), p.type, p.data);
	}
}

}
