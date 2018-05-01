/*
  Created by Jenny White on 29.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "BaseMenuObject.hpp"
#include "Container.hpp"
#include "Label.hpp"

class Variable: public Container
{
public:
    inline explicit Variable(BaseMenuObject *parent)
            : Container(parent)
    {
        ySize = 12;
    }

    inline virtual void notifyMove()
    {
        xSize = parent->xSize;
        Container::notifyMove();
    }

    Label *label{ nullptr };
};