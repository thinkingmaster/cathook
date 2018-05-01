/*
  Created by Jenny White on 29.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <glez/draw.hpp>
#include "BaseMenuObject.hpp"
#include "menu.hpp"

class Checkbox: public BaseMenuObject
{
public:
    inline explicit Checkbox(BaseMenuObject *parent): BaseMenuObject(parent)
    {
        xSize = 12;
        ySize = 12;
    }

    inline virtual bool handleSdlEvent(SDL_Event *event)
    {
        if (event->type == SDL_MOUSEBUTTONDOWN)
        {
            int mx, my;
            SDL_GetMouseState(&mx, &my);
            if (isInBounds(mx, my))
            {
                checked = !checked;
            }
        }

        return false;
    }
    inline virtual void render()
    {
        glez::draw::rect_outline(x, y, 12, 12, resources().c_foreground, 1.0f);

        if (checked)
            glez::draw::rect(x + 3, y + 3, 6, 6, resources().c_foreground_2);
    }

    bool checked;
};