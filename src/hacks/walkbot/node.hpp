/*
 * node.hpp
 *
 *  Created on: Aug 20, 2017
 *      Author: nullifiedcat
 */

#pragma once

#include "common.h"

namespace hacks { namespace shared { namespace walkbot {

class Node {
public:
	struct options_t {
		bool jump { false };
		bool crouch { false };
	};
	struct connection_t {
		std::shared_ptr<Node> target;
		std::shared_ptr<ComplexCondition> condition;
	};
public:
	Node(const nlohmann::json& json);
	operator nlohmann::json() const;

	void resolve_connections();

public:
	float x { 0.0f };
	float y { 0.0f };
	float z { 0.0f };

	std::vector<connection_t> connections {};
	options_t options {};
};

}}}
