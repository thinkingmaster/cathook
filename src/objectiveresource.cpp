/*
 * objectiveresource.cpp
 *
 *  Created on: Aug 20, 2017
 *      Author: nullifiedcat
 */

#include "objectiveresource.hpp"
#include "common.h"

namespace objectives {

int eid { 0 };

void Update() {
	eid = 0;
	for (int i = 1; i < entity_cache::max; i++) {
		CachedEntity* e = ENTITY(i);
		if (CE_GOOD(e)) {
			if (e->m_iClassID == CL_CLASS(CTFObjectiveResource)) {
				eid = i;
				return;
			}
		}
	}
}

int GetOwner(int point) {
	if (point < 0 || point >= 8) return 0;
	if (eid) {
		CachedEntity* e = ENTITY(eid);
		if (CE_GOOD(e) && e->m_iClassID == CL_CLASS(CTFObjectiveResource)) {
			return CE_INT(e, netvar.m_iOwner + point * 4);
		}
	}
	return 0;
}

}
