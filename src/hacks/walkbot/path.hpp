/*
 * path.hpp
 *
 *  Created on: Aug 20, 2017
 *      Author: nullifiedcat
 */

#pragma once

#include "common.h"
#include "node.hpp"

namespace hacks { namespace shared { namespace walkbot {

class Node;

class Path {
public:
	typedef Handle<Path, uuid_t> handle_t;
public:
	Path(Pathing& parent, const nlohmann::json& json);
	operator nlohmann::json() const;

public:
	uuid_t uuid {};
	std::string name { "Unnamed" };
	std::unordered_map<int, std::shared_ptr<Node>> nodes {};
};

}}}
