/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "ncc/BaseMenuObject.hpp"
#include <functional>
#include <ncc/Message.hpp>

namespace zerokernel
{

class Button: public BaseMenuObject
{
public:
    inline Button(): BaseMenuObject{}
    {
        xSize = 60;
        ySize = 14;
    }

    inline bool handleSdlEvent(SDL_Event *event) override
    {
        if (event->type == SDL_MOUSEBUTTONDOWN)
        {
            if (isHovered())
            {
                onButtonClick();
                return true;
            }
        }
        return false;
    }

protected:
    inline virtual void onButtonClick()
    {
        Message msg{"ButtonClicked"};
        emit(msg);
    }
};

}