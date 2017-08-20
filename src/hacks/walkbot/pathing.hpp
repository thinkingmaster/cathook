/*
 * pathfile.hpp
 *
 *  Created on: Aug 20, 2017
 *      Author: nullifiedcat
 */

#pragma once

#include "common.h"

namespace hacks { namespace shared { namespace walkbot {

class Pathing {
public:
	class ConditionalPath {
	public:
		ConditionalPath(const Pathing&, const nlohmann::json&);
		operator nlohmann::json() const;

		bool pass() const;
		Path& get() const;
	public:
		nlohmann::json json {};
		Pathing& parent;
	};

public:
	Pathing(const nlohmann::json&);
	operator nlohmann::json() const;

public:
	int version { 2 };
	nlohmann::json author {};
	std::string map { "" };
	std::vector<ConditionalPath> path_conditions {};
	std::unordered_map<std::string, ComplexCondition> conditions {};
	std::unordered_map<std::string, Path> pathes {};
};

}}}
