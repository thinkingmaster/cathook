
#include "xorstring.hpp"

/*
 * utfccp_commands.cpp
 *
 *  Created on: Jul 1, 2017
 *      Author: nullifiedcat
 */

#include "ucccccp.hpp"
#include "common.h"
#include "ucccccp_cmds.hpp"

CatCommand utfccp_encrypt(XStr("ucccccp_encrypt"), XStr("Encrypt a message"), [](const CCommand& args) {
	logging::Info(XStr("%s"), ucccccp::encrypt(std::string(args.ArgS())).c_str());
});

CatCommand utfccp_decrypt(XStr("ucccccp_decrypt"), XStr("Decrypt a message"), [](const CCommand& args) {
	if (ucccccp::validate(std::string(args.ArgS()))) {
		logging::Info(XStr("%s"), ucccccp::decrypt(std::string(args.ArgS())).c_str());
	} else {
		logging::Info(XStr("Invalid input data!"));
	}
});
