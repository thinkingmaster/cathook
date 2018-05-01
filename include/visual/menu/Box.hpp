/*
  Created by Jenny White on 29.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "Container.hpp"
#include "CenteredString.hpp"
#include "List.hpp"
#include "menu.hpp"
#include <glez/glez.hpp>

class Box: public BaseMenuObject
{
public:
    inline explicit Box(BaseMenuObject *parent): BaseMenuObject(parent), title(this), list(this)
    {
        list.setOffset(8, 10);
        title.setOffset(0, 0);
    }

    inline virtual void render()
    {
        auto& rx = resources();
        title.x = x;
        title.y = y;
        title.xSize = xSize;
        title.ySize = 14;
        title.render();
        //glez::draw::rect(x, y, xSize, ySize, glez::color::black);
        // top left
        glez::draw::line(x + 2, y + 5, (xSize - title.xSizeReal) / 2 - 5, 0, rx.c_foreground, 1.0f);
        // top right
        glez::draw::line(x + xSize - 3, y + 5, -(xSize - title.xSizeReal) / 2 + 5, 0, rx.c_foreground, 1.0f);
        // left
        glez::draw::line(x + 2, y + 5, 0, ySize - 8, rx.c_foreground, 1.0f);
        // bottom
        glez::draw::line(x + 2, y + ySize - 3, xSize - 5, 0, rx.c_foreground, 1.0f);
        // right
        glez::draw::line(x + xSize - 3, y + ySize - 3, 0, -ySize + 7, rx.c_foreground, 1.0f);
        list.render();
    }
    virtual bool handleSdlEvent(SDL_Event *event)
    {
        return list.handleSdlEvent(event);
    }
    virtual void notifyMove()
    {
        list.childUpdateLocation(x, y);
        title.childUpdateLocation(x, y);
        xSize = list.xSize + 16;
        ySize = list.ySize + 16;
    }

    CenteredString title;
    List list;
};