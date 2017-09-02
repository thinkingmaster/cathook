/*
 * pathfile.hpp
 *
 *  Created on: Aug 20, 2017
 *      Author: nullifiedcat
 */

#pragma once

#include "common.h"

#include "path.hpp"
#include "condition.hpp"

namespace hacks { namespace shared { namespace walkbot {

class Pathing {
public:
public:
	Pathing();

	nlohmann::json toJSON() const;
	bool fromJSON(const nlohmann::json& json);

	void reset();

	bool save(const std::string& map, const std::string& name);
	bool load(const std::string& map, const std::string& name);

	Path& active() const;
public:
	int version { 3 };
	nlohmann::json author {};
	std::string map { "" };
	std::vector<std::pair<Path::Handle, ComplexCondition::Handle>> path_descriptors {};
	std::unordered_map<uuid_t, ComplexCondition, uuid_t::hash> conditions {};
	std::unordered_map<uuid_t, Path, uuid_t::hash> paths {};
};

}}}
