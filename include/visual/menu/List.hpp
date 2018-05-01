/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "Container.hpp"

class List: public Container
{
public:
    inline explicit List(BaseMenuObject *parent): Container(parent)
    {
        xSize = 300;
    }

    inline virtual void notifyMove()
    {
        orderVerticalListAndUpdateSize();
    }

    inline void orderVerticalListAndUpdateSize()
    {
        int c{ 0 };
        bool first{ true };
        for (auto& object: objects)
        {
            if (not first)
                c += interval;
            object->childMove(0, c, x, y);
            c += object->ySize;
            first = false;
        }
        ySize = c;
    }

    int interval{ 3 };
};