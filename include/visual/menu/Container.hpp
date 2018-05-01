/*
  Created by Jenny White on 29.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <memory>
#include <vector>
#include "BaseMenuObject.hpp"

class Container: public BaseMenuObject
{
public:
    Container(BaseMenuObject *parent)
            : BaseMenuObject(parent)
    {
    }

    inline virtual bool handleSdlEvent(SDL_Event *event)
    {
        for (auto& object: objects)
        {
            if (object->handleSdlEvent(event))
                return true;
        }
        return false;
    }
    inline virtual void render()
    {
        //glez::draw::rect(x, y, xSize, ySize, glez::rgba(200, 0, 0, 50));
        for (auto& object: objects)
        {
            object->render();
        }
    }
    inline virtual void notifyMove()
    {
        for (auto& object: objects)
        {
            object->childUpdateLocation(x, y);
        }
    }


    inline void addObject(std::unique_ptr<BaseMenuObject>&& object)
    {
        object->parent = this;
        objects.push_back(std::move(object));
    }
public:
    std::vector<std::unique_ptr<BaseMenuObject>> objects{};
};