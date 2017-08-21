/*
 * path.hpp
 *
 *  Created on: Aug 20, 2017
 *      Author: nullifiedcat
 */

#pragma once

#include "common.h"

#include "pathing.hpp"

namespace hacks { namespace shared { namespace walkbot {

class Node {
public:
	typedef Handle<Node, int> handle_t;
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
	struct options_t {
		bool jump { false };
		bool crouch { false };
	};
public:
	Node(Path&, const nlohmann::json& json);
	operator nlohmann::json() const;

	bool connected(const Node&) const;

	inline Vector& xyz() {
		return *reinterpret_cast<Vector*>(&x);
	}

	void link(handle_t node, bool both);
	void unlink(handle_t node, bool both);
public:
	float x { 0.0f };
	float y { 0.0f };
	float z { 0.0f };

	int id { 0 };

	std::vector<Connection> connections {};
	options_t options {};
	Path& parent_;
};

class Path {
public:
	typedef Handle<Path, uuid_t> handle_t;
public:
	Path();
	Path(const uuid_t& uuid, const nlohmann::json& json);
	operator nlohmann::json() const;

	Node::handle_t create(const Vector& xyz);
	void 		   remove(Node::handle_t node);
public:
	std::string name { "Unnamed" };
	std::unordered_map<int, std::shared_ptr<Node>> nodes {};
};

}}}
