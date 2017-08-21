/*
 * Walkbot.hpp
 *
 *  Created on: Jul 23, 2017
 *      Author: nullifiedcat
 */

#pragma once

#include "walkbot/pathing.hpp"

namespace hacks { namespace shared { namespace walkbot {

Pathing& pathing();

void Initialize();
#if ENABLE_VISUALS == 1
void Draw();
#endif
void Move();
void OnLevelInit();

}}}
