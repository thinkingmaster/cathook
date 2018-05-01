/*
 * drawing.h
 *
 *  Created on: Oct 5, 2016
 *      Author: nullifiedcat
 */

#pragma once

#include "common.hpp"

#include <glez/glez.hpp>

class CachedEntity;
class Vector;
class CatVar;
class IClientEntity;
class CatEnum;
class VMatrix;

namespace fonts
{

extern glez::font main_font;
}

constexpr rgba_t GUIColor()
{
    return colors::white;
}

void InitStrings();
void ResetStrings();
void AddCenterString(const std::string &string,
                     const rgba_t &color = colors::white);
void AddSideString(const std::string &string,
                   const rgba_t &color = colors::white);
void DrawStrings();

namespace draw
{

extern std::mutex draw_mutex;
extern VMatrix wts;

extern int width;
extern int height;
extern float fov;

void Initialize();

void UpdateWTS();
bool WorldToScreen(const Vector &origin, Vector &screen);
bool EntityCenterToScreen(CachedEntity *entity, Vector &out);
}
