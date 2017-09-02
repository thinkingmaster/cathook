/*
 * condition.hpp
 *
 *  Created on: Aug 20, 2017
 *      Author: nullifiedcat
 */

#pragma once

#include "common.h"

namespace hacks { namespace shared { namespace walkbot {

class ConditionPart {
public:
	ConditionPart(ComplexCondition&, const nlohmann::json& json);
	operator nlohmann::json() const;

	bool pass() const;
public:
	nlohmann::json json_;
	ComplexCondition& parent_;
};

class ComplexCondition {
public:
	class Handle {
	public:
		inline Handle() : empty_(true) {}

		inline void reset() {
			empty_ = true;
		}
		// must return false if empty_
		bool good() const;
		// must return nullptr if empty_
		ComplexCondition* get() const;
		// must set empty_ to false
		inline Handle& operator=(uuid_t key) {
			key_ = key;
			empty_ = false;
			return *this;
		}
		// sets empty_ to false (if handle isn't empty)
		inline Handle& operator=(const Handle& other) {
			key_ = other.key_;
			empty_ = other.empty_;
			return *this;
		}
		inline bool operator==(const Handle& other) const {
			return empty_ ? other.empty_ : other.key_ == key_;
		}
		// Shortcuts
		inline operator bool() const {
			return good();
		}
		inline operator ComplexCondition*() const {
			return get();
		}
		inline ComplexCondition* operator->() const {
			return get();
		}
	public:
		bool empty_ { true };
		uuid_t key_;
	};
public:
	ComplexCondition();

	nlohmann::json toJSON() const;
	void fromJSON(const uuid_t& uuid, const nlohmann::json& json);

	bool pass() const;
public:
	uuid_t uuid_ {};
	std::vector<ConditionPart> conditions_ {};
};

}}}


