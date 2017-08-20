/*
 * pathfile.hpp
 *
 *  Created on: Aug 20, 2017
 *      Author: nullifiedcat
 */

#pragma once

#include "common.h"

namespace hacks { namespace shared { namespace walkbot {

class Pathfile {
public:
	Pathfile(const nlohmann::json&);
	operator nlohmann::json() const;

public:

};

}}}
