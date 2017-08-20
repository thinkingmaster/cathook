/*
 * pathfile.hpp
 *
 *  Created on: Aug 20, 2017
 *      Author: nullifiedcat
 */

#pragma once

#include "common.h"

namespace hacks { namespace shared { namespace walkbot {

class Pathing;

template<typename T, typename K>
class Handle {
public:
	Handle(Pathing&);

	bool good() const;
	T& get() const;
	T& operator=(const T&);
	T& operator=(const K&);
public:
	Pathing& parent_;
};

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

	Path& get() const;
public:
	int version { 3 };
	nlohmann::json author {};
	std::string map { "" };
	std::vector<ConditionalPath> path_descriptors {};
	std::unordered_map<uuid_t, ComplexCondition, uuid_t::hash> conditions {};
	std::unordered_map<uuid_t, Path, uuid_t::hash> pathes {};
};

}}}
