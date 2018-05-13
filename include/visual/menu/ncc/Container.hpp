/*
  Created by Jenny White on 29.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <memory>
#include <vector>
#include "BaseMenuObject.hpp"
#include <glez/glez.hpp>
#include <algorithm>


namespace zerokernel
{

class Container : public BaseMenuObject
{
public:
    inline Container(): BaseMenuObject{}
    {
    }

    inline virtual bool handleSdlEvent(SDL_Event *event)
    {
        cleanupObjects();
        for (auto &object: objects)
        {
            object->handleSdlEvent(event);
            //if (object->handleSdlEvent(event))
            //    return true;
        }
        return false;
    }

    inline virtual void render()
    {
        //glez::draw::rect(x, y, xSize, ySize, glez::rgba(200, 0, 0, 50));
        for (auto &object: objects)
        {
            object->render();
        }

        BaseMenuObject::render();
    }

    inline virtual void notifyMove()
    {
        for (auto &object: objects)
        {
            object->childUpdateLocation(x, y);
        }
    }

    inline virtual void update()
    {
        cleanupObjects();
        for (auto &object: objects)
            object->update();
    }

    inline virtual void notifySize()
    {
        cleanupObjects();
        for (auto &object: objects)
            object->notifySize();
    }

    void loadFromXml(const tinyxml2::XMLElement *data) override;

    inline void addObject(std::unique_ptr<BaseMenuObject> &&object)
    {
        object->setParent(this);
        objects.push_back(std::move(object));
    }

    inline void cleanupObjects()
    {
        objects.erase(std::remove_if(objects.begin(), objects.end(),
                                     [](std::unique_ptr<BaseMenuObject> &o) {
                                         return o->markedForDelete;
                                     }), objects.end());
    }

    void setParent(BaseMenuObject *parent) override
    {
        BaseMenuObject::setParent(parent);
        for (auto& object: objects)
            object->setParent(this);
    }

    virtual void reset()
    {
        objects.clear();
    }

public:
    std::vector<std::unique_ptr<BaseMenuObject>> objects{};
};

}