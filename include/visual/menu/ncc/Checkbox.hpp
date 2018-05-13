/*
  Created by Jenny White on 29.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <glez/draw.hpp>
#include <iostream>
#include "BaseMenuObject.hpp"
#include "Menu.hpp"
#include "Button.hpp"
#include "ComboboxList.hpp"

namespace zerokernel
{

class Checkbox : public Button
{
public:
    inline Checkbox(): Button{}
    {
        xSize = 12;
        ySize = 12;
    }

    inline virtual void render()
    {
        glez::draw::rect_outline(x, y, xSize, ySize, resource::color::foreground, 1.0f);

        if (checked)
            glez::draw::rect(x + 3, y + 3, xSize - 6, ySize - 6, resource::color::foreground_2);

        BaseMenuObject::render();
    }

    bool checked{ false };
protected:
    inline void onButtonClick() override
    {
        Button::onButtonClick();
        checked = !checked;
    }
};

}