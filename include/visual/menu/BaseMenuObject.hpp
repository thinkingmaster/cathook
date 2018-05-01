/*
  Created by Jenny White on 29.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <SDL2/SDL_events.h>

enum class SizePolicy
{
    FIXED,
    FILL
};

class BaseMenuObject
{
public:
    explicit BaseMenuObject(BaseMenuObject *parent)
            : parent(parent)
    {
    }

    virtual ~BaseMenuObject() = default;
    inline virtual bool handleSdlEvent(SDL_Event *event)
    {
        return false;
    }
    inline virtual void render()
    {
    }
    inline virtual void notifyMove()
    {
    }
    /* Just sets the offset */
    inline void setOffset(int x, int y)
    {
        xOffset = x;
        yOffset = y;
    }
    /* Moves the object, ax/ay are considered to be absolute location of the parent object */
    inline void childMove(int x, int y, int ax, int ay)
    {
        xOffset = x;
        yOffset = y;
        childUpdateLocation(ax, ay);
    }
    /*
     * Updates the location using ax/ay
     */
    inline void childUpdateLocation(int ax, int ay)
    {
        x = xOffset + ax;
        y = yOffset + ay;
        notifyMove();
    }
    /*
     * Explicitly moves the object - updates location and calls childMove on children.
     */
    inline void move(int x, int y)
    {
        xOffset = x;
        yOffset = y;
        updateLocation();
    }
    inline void updateLocation()
    {
        this->x = xOffset;
        this->y = yOffset;
        BaseMenuObject *p = parent;
        while (p)
        {
            x += p->xOffset;
            y += p->yOffset;
            p = p->parent;
        }
        notifyMove();
    }
    inline bool isInBounds(int xp, int yp)
    {
        return xp >= x && xp <= x + xSize &&
                         yp >= y && yp <= y + ySize;
    }

    int x{ 0 };
    int y{ 0 };
    int xOffset{ 0 };
    int yOffset{ 0 };
    int xSize{ 0 };
    int ySize{ 0 };
    BaseMenuObject *parent{ nullptr };
};