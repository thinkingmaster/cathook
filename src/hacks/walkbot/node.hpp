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
	class Connection {
	public:
		Connection(nlohmann::json json);
		operator nlohmann::json() const;

		void resolve();
	public:
		uuid_t uuid_ {};
		std::shared_ptr<Node> target { nullptr };
		std::shared_ptr<ComplexCondition> condition { nullptr };
	};
public:
	Node(const nlohmann::json& json);
	operator nlohmann::json() const;

	void resolve_connections();
	bool connected(const Node&) const;

	inline Vector& xyz() {
		return *reinterpret_cast<Vector*>(&x);
	}
public:
	float x { 0.0f };
	float y { 0.0f };
	float z { 0.0f };

	int id { 0 };

	std::vector<Connection> connections {};
	options_t options {};
};

}}}
