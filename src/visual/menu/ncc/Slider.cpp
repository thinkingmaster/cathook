/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include "ncc/Slider.hpp"
#include "ncc/Menu.hpp"

namespace zerokernel
{

bool Slider::handleSdlEvent(SDL_Event *event)
{
    if (event->type == SDL_MOUSEBUTTONDOWN)
    {
        if (isHovered())
        {
            setValue(Menu::instance->mouseY - y);
            grabbed = true;
        }
    } else if (event->type == SDL_MOUSEBUTTONUP)
        grabbed = false;

    return BaseMenuObject::handleSdlEvent(event);
}

void Slider::render()
{
    glez::draw::rect(x + 7, y, 2, ySize, resource::color::foreground);
    glez::draw::rect(x, y + sliderLocation - 6, 16, 8,
                     resource::color::background);
    glez::draw::rect_outline(x, y + sliderLocation - 6, 16, 8,
                             resource::color::foreground, 1);

    BaseMenuObject::render();
}

void Slider::update()
{
    if (!grabbed)
        return;

    setValue(sliderLocation + Menu::instance->dy);
}

Slider::Slider(): BaseMenuObject{}
{
    ySize = 100;
    xSize = 16;
}

void Slider::setValue(int value)
{
    sliderLocation = value;
    if (sliderLocation < 0)
        sliderLocation = 0;
    else if (sliderLocation > ySize)
        sliderLocation = ySize;
}

}