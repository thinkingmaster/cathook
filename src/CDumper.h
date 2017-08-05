
#include "xorstring.hpp"

/*
 * CDumper.h
 *
 *  Created on: Oct 5, 2016
 *      Author: nullifiedcat
 */

#ifndef CDUMPER_H_
#define CDUMPER_H_

#include "beforecheaders.h"
#include <fstream>
#include <string>
#include "aftercheaders.h"

#include "logging.h"
#include "fixsdk.h"
#include <dt_common.h>
#include <client_class.h>

class CDumper {
public:
	CDumper() {
		m_file.open(XStr("/tmp/netdump.txt"), std::ios::out | std::ios::trunc);
	}

	~CDumper() {
		m_file.close();
	}

	const std::string TypeToString(SendPropType type) const {
		switch (type) {
		case DPT_Int:
			return XStr("INT");
		case DPT_Float:
			return XStr("FLOAT");
		case DPT_Vector:
			return XStr("VECTOR3");
		case DPT_VectorXY:
			return XStr("VECTOR2");
		case DPT_Array:
			return XStr("ARRAY");
		case DPT_String:
			return XStr("STRING");
		case DPT_DataTable:
			return XStr("TABLE");
		default:
			return XStr("UNKNOWN");
		}
	}

	void SaveDump() {
		ClientClass *pList = g_IBaseClient->GetAllClasses();
		while (pList != nullptr) {
			DumpTable(pList->m_pRecvTable, 0);
			pList = pList->m_pNext;
		}
		m_file.close();
	}

	void DumpTable(RecvTable *pTable, int iLevel, int parent_offset = 0) {
		if (pTable == nullptr) {
			return;
		}

		for (int j = 0; j < iLevel; j++) {
			m_file << XStr("\t");
		}

		m_file << pTable->GetName() << XStr("\n");

		++iLevel;

		for(int i = 0; i < pTable->GetNumProps(); ++i) {
			RecvProp *pProp = pTable->GetProp(i);
			if (pProp == nullptr) {
				continue;
			}

			if (isdigit(pProp->GetName()[0])) {
				continue;
			}

			if (pProp->GetDataTable()) {
				DumpTable(pProp->GetDataTable(), iLevel + 1, pProp->GetOffset());
			}

			for(int j = 0; j < iLevel; j++)
				m_file << XStr("\t");

			int offset = pProp->GetOffset();
			SendPropType type = pProp->GetType();

			m_file << pProp->GetName() << XStr(" : 0x") << std::hex << offset << XStr(" (0x") << (parent_offset + offset) << XStr(") [") << TypeToString(type) << XStr("]") << XStr("\n");
		}

		if (iLevel == 2)
			m_file << std::endl;
	}
private:
	std::fstream m_file;
};


#endif /* CDUMPER_H_ */
