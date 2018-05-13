/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <glez/draw.hpp>
#include "Button.hpp"
#include "Menu.hpp"


namespace zerokernel
{

class InputButton : public BaseMenuObject
{
public:
    inline explicit InputButton(): BaseMenuObject{}
    {
        xSize = 60;
        ySize = 14;
    }

    inline virtual bool handleSdlEvent(SDL_Event *event) override
    {
        if (capturing && event->type == SDL_KEYDOWN)
        {
            if (event->key.keysym.scancode != SDLK_ESCAPE)
            {
                updateKey(event->key.keysym.scancode);
            }
            capturing = false;
            return true;
        }

        if (event->type == SDL_MOUSEBUTTONDOWN)
        {
            if (capturing)
            {
                updateKey(SDL_NUM_SCANCODES + event->button.button);
                capturing = false;
                return true;
            }
            if (isHovered())
            {
                capturing = true;
                return true;
            }

        }
        return false;
    }

    inline virtual void render() override
    {
        glez::draw::rect_outline(x, y, xSize, ySize,
                                 capturing ? resource::color::foreground_2
                                           : resource::color::foreground, 1);
        utility::drawCenteredString(x + xSize / 2, y + 3, keyname, resource::font::bold, resource::color::text);

        BaseMenuObject::render();
    }

    void updateKey(int key)
    {
        if (key < SDL_NUM_SCANCODES)
        {
            const char *name = SDL_GetScancodeName((SDL_Scancode) key);
            if (!name || strlen(name) == 0)
                keyname = std::to_string(key);
            else
                keyname = std::string(name);
        } else
            keyname = "MOUSE " + std::to_string(key - SDL_NUM_SCANCODES);
    }

    bool capturing{false};
    std::string keyname{};

    int key{0};
};

}