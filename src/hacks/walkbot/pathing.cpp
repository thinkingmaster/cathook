/*
 * pathing.cpp
 *
 *  Created on: Aug 22, 2017
 *      Author: nullifiedcat
 */

#include "pathing.hpp"

namespace hacks { namespace shared { namespace walkbot {

Pathing::Pathing() {};

nlohmann::json Pathing::toJSON() const {
	nlohmann::json result {};
	result["version"] = version;
	result["author"] = author;
	result["map"] = map;

	nlohmann::json out_conditions {};
	nlohmann::json out_descriptors {};
	nlohmann::json out_paths {};

	for (const auto& k : conditions) {
		out_conditions[std::string(k.first)] = k.second.toJSON();
	}
	for (const auto& k : path_descriptors) {
		out_descriptors.push_back(nlohmann::json {
			{ "path", std::string(k.first.key_) },
			{ "condition", k.second ? std::string(k.second.key_) : /* null */ nlohmann::json {} }
		});
	}
	for (const auto& k : paths) {
		out_paths[std::string(k.first)] = k.second.toJSON();
	}

	result["conditions"] = out_conditions;
	result["descriptors"] = out_descriptors;
	result["paths"] = out_paths;

	return result;
}

bool Pathing::fromJSON(const nlohmann::json& json) {
	reset();

	version = json["version"].get<int>();
	author = json["author"];
	map = json["map"];

	for (const auto& k : json[""]) {

	}

	return true;
}

void Pathing::reset() {
	version = 0;
	author = nlohmann::json {};
	map = "unknown";
	path_descriptors.clear();
	conditions.clear();
	paths.clear();
}

bool Pathing::save(const std::string& map, const std::string& name) {
	if (not DirectoryExists(DATA_PATH + "/walkbot")) {
		mkdir(DATA_PATH "/walkbot", S_IRWXU | S_IRWXG);
	}
	if (not DirectoryExists(DATA_PATH + "/walkbot/" + map)) {
		return false;
	}
	return false;
}

bool Pathing::load(const std::string& map, const std::string& name) {
	if (not DirectoryExists(DATA_PATH + "/walkbot/" + map)) {
		return false;
	}
	return false;
}

}}}
