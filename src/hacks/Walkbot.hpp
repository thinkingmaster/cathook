/*
 * Walkbot.hpp
 *
 *  Created on: Jul 23, 2017
 *      Author: nullifiedcat
 */

#pragma once

namespace hacks { namespace shared { namespace walkbot {

void Initialize();
#if not NO_RENDERING
void Draw();
#endif
void Move();
void OnLevelInit();

}}}
