/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "BaseMenuObject.hpp"

class TabButton: public BaseMenuObject
{
public:
    inline explicit TabButton(BaseMenuObject *parent): BaseMenuObject(parent)
    {
    }

    inline virtual bool handleSdlEvent(SDL_Event *event) override
    {
        return BaseMenuObject::handleSdlEvent(event);
    }

    inline virtual void render() override
    {
        BaseMenuObject::render();
    }
};