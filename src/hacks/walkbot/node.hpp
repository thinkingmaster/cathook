/*
 * node.hpp
 *
 *  Created on: Aug 20, 2017
 *      Author: nullifiedcat
 */

#pragma once

#include "common.h"
#include "condition.hpp"
#include "pathing.hpp"
#include "path.hpp"

namespace hacks { namespace shared { namespace walkbot {

class Node {
public:
	typedef Handle<Node, int> handle_t;
	struct options_t {
		bool jump { false };
		bool crouch { false };
	};
	class Connection {
	public:
		Connection(nlohmann::json json);
		Connection(handle_t node);
		operator nlohmann::json() const;

		bool prioritized() const;
		bool available() const;
	public:
		Node::handle_t target;
		nlohmann::json options;
		std::shared_ptr<ComplexCondition> condition { nullptr };
	};
public:
	Node(Path&, const nlohmann::json& json);
	operator nlohmann::json() const;

	bool connected(const Node&) const;

	inline Vector& xyz() {
		return *reinterpret_cast<Vector*>(&x);
	}

	void link(handle_t node);
	void unlink(handle_t node);
public:
	float x { 0.0f };
	float y { 0.0f };
	float z { 0.0f };

	int id { 0 };

	std::vector<Connection> connections {};
	options_t options {};
	Path& parent_;
};

}}}
