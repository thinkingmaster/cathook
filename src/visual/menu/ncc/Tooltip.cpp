/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include "Tooltip.hpp"
#include <glez/glez.hpp>
#include "Menu.hpp"

namespace zerokernel
{

Tooltip::Tooltip() : BaseMenuObject{}
{

}

void Tooltip::render()
{
    SDL_GetMouseState(&x, &y);
    x += 6;

    glez::draw::rect(x, y, xSize, ySize, resource::color::background_tooltip);
    glez::draw::rect_outline(x, y, xSize, ySize, resource::color::foreground, 1);
    glez::draw::outlined_string(x + 2, y + 3, text, resource::font::base,
                                resource::color::text, resource::color::text_shadow,
                                1, nullptr, nullptr);

    BaseMenuObject::render();
}

void Tooltip::setText(std::string text)
{
    if (text == lastText)
        return;

    lastText = text;
    int lc;
    int width;
    this->text = utility::wrapString(text, resource::font::base, 300, &width,
                                     &lc);
    xSize = width + 8;
    ySize = 4 + lc * int(resource::font::base.size + 1);
}

}