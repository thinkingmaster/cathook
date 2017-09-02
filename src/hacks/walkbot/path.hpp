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
	class Handle {
	public:
		inline Handle() : empty_(true) {}

		inline void reset() {
			empty_ = true;
		}
		// must return false if empty_
		bool good() const;
		// must return nullptr if empty_
		Node* get() const;
		// must set empty_ to false
		// Handle<T, K>& operator=(const T&) = delete;
		inline Handle& operator=(int key) {
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
		inline operator Node*() const {
			return get();
		}
		inline Node* operator->() const {
			return get();
		}
	public:
		bool empty_ { true };
		int key_;
	};
	class Connection {
	public:
		Connection(nlohmann::json json);
		Connection(Handle node);
		operator nlohmann::json() const;

		bool prioritized() const;
		bool available() const;
	public:
		Node::Handle target;
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

	void link(Node::Handle node, bool both);
	void unlink(Node::Handle node, bool both);
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
	class Handle {
	public:
		inline Handle() : empty_(true) {}

		inline void reset() {
			empty_ = true;
		}
		// must return false if empty_
		bool good() const;
		// must return nullptr if empty_
		Path* get() const;
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
		inline operator Path*() const {
			return get();
		}
		inline Path* operator->() const {
			return get();
		}
	public:
		bool empty_ { true };
		uuid_t key_ {};
	};
public:
	Path();

	nlohmann::json toJSON() const;
	void fromJSON(const uuid_t& uuid, const nlohmann::json& json);

	Node::Handle create(const Vector& xyz);
	void 		 remove(Node::Handle node);
public:
	uuid_t uuid_ {};
	std::string name { "Unnamed" };
	std::unordered_map<int, Node> nodes {};
};

}}}
