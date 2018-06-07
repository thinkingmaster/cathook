/*
 * Radar.hpp
 *
 *  Created on: Mar 28, 2017
 *      Author: nullifiedcat
 */

#pragma once
#if ENABLE_VISUALS
#include "visual/atlas.hpp"
#include "common.hpp"

namespace hacks
{
namespace tf
{
namespace radar
{
std::pair<int, int> WorldToRadar(int x, int y);
void Draw();
}
}
}
#endif
