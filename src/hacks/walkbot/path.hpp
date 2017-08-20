/*
 * path.hpp
 *
 *  Created on: Aug 20, 2017
 *      Author: nullifiedcat
 */

#pragma once

#include "common.h"

namespace hacks { namespace shared { namespace walkbot {

class Path {
public:
	Path(const nlohmann::json& json);
	operator nlohmann::json() const;

	void resolve_nodes();
public:
	std::string name { "Unnamed" };
	std::unordered_map<int, std::shared_ptr<Node>> nodes {};
};

}}}
