/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "List.hpp"
#include "Menu.hpp"
#include "Combobox.hpp"


namespace zerokernel
{

class ComboboxList: public List
{
public:
    ComboboxList(Combobox *box);

    inline virtual bool handleSdlEvent(SDL_Event *event) override
    {
        if (event->type == SDL_KEYDOWN || !isHovered() && event->type == SDL_MOUSEBUTTONDOWN)
        {
            markedForDelete = true;
            box->onOverlayClose();
            return true;
        }
        return Container::handleSdlEvent(event);
    }

    inline virtual void render() override
    {
        glez::draw::rect(x, y, xSize, ySize, resource::color::background_tooltip);
        Container::render();
    }

    Combobox *box;

};

}