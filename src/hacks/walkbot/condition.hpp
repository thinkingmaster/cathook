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
	ConditionPart(const nlohmann::json& json);
	operator nlohmann::json() const;

	bool pass() const;
public:
	nlohmann::json json_;
};

class ComplexCondition {
public:
	ComplexCondition(nlohmann::json json);
	operator nlohmann::json() const;

	bool pass() const;
public:
	std::vector<ConditionPart> conditions_ {};
	uuid_t uuid_ {};
};

}}}


