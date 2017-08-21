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

class Pathing;

template<typename T, typename K>
class Handle {
public:
	inline Handle(Pathing& parent) : empty_(true) {}

	inline void reset() {
		empty_ = true;
	}
	// must return false if empty_
	bool good() const = delete;
	// must throw if empty_
	T& get() const = delete;
	// must set empty_ to false
	Handle<T, K>& operator=(const T&) = delete;
	inline Handle<T, K>& operator=(const K& key) {
		key_ = key;
		empty_ = false;
		return *this;
	}
	// sets empty_ to false (if handle isn't empty)
	inline Handle<T, K>& operator=(const Handle<T, K>& other) {
		key_ = other.key_;
		empty_ = other.empty_;
		return *this;
	}
	inline bool operator==(const Handle<T, K>& other) const {
		return empty_ ? other.empty_ : other.key_ == key_;
	}
	inline T& operator()() const {
		return get();
	}
	inline operator bool() const {
		return not empty_;
	}
	inline operator T&() const {
		return get();
	}
public:
	bool empty_ { true };
	K key_;
};

class Pathing {
public:
	class ConditionalPath {
	public:
		ConditionalPath(const nlohmann::json&);
		operator nlohmann::json() const;

		bool pass() const;
	public:
		Path::handle_t path;
		nlohmann::json json {};
	};

public:
	Pathing();
	Pathing(const nlohmann::json&);
	operator nlohmann::json() const;

	void reset();

	bool save(const std::string&);
	bool load(const std::string&);

	Path& active() const;
public:
	int version { 3 };
	nlohmann::json author {};
	std::string map { "" };
	std::vector<ConditionalPath> path_descriptors {};
	std::unordered_map<uuid_t, ComplexCondition, uuid_t::hash> conditions {};
	std::unordered_map<uuid_t, Path, uuid_t::hash> pathes {};
};

}}}
