
#include "../xorstring.hpp"

/*
 * dump.cpp
 *
 *  Created on: May 13, 2017
 *      Author: nullifiedcat
 */

#include "dump.hpp"
#include "../common.h"

static CatCommand do_dump(XStr("debug_dump_classes"), XStr("Dump classes"), PerformClassDump);

void PerformClassDump() {
	ClientClass* cc = g_IBaseClient->GetAllClasses();
	FILE* cd = fopen(XStr("/tmp/cathook-classdump.txt"), XStr("w"));
	if (cd) {
		while (cc) {
			fprintf(cd, XStr("[%d] %s\n"), cc->m_ClassID, cc->GetName());
			cc = cc->m_pNext;
		}
		fclose(cd);
	}
}

static CatCommand populate_dynamic(XStr("debug_populate_dynamic"), XStr("Populate dynamic class table"), []() {
	client_classes::dynamic_list.Populate();
});
