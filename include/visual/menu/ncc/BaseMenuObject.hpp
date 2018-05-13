/*
  Created by Jenny White on 29.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <SDL2/SDL_events.h>
#include <ncc/interface/IMessageHandler.hpp>
#include <vector>
#include "tinyxml2.h"

namespace zerokernel
{

class Menu;

class BaseMenuObject: public IMessageHandler
{
public:
    static std::size_t objectCount;

    inline virtual ~BaseMenuObject()
    {
        printf("~BaseMenuObject %u\n", --objectCount);
    }

    inline BaseMenuObject()
    {
        printf("BaseMenuObject() %u\n", ++objectCount);
    }

    virtual bool handleSdlEvent(SDL_Event *event);

    virtual void render();

    virtual void update();

    virtual void notifyMove();

    virtual void notifySize();

    virtual void setParent(BaseMenuObject *parent);

    virtual void loadFromXml(const tinyxml2::XMLElement *data);

    virtual bool isHidden();

    void handleMessage(Message &msg) override;

    // End of virtual methods

    /* Just sets the offset */
    void setOffset(int x, int y);

    /* Moves the object, ax/ay are considered to be absolute location of the parent object */
    void childMove(int x, int y, int ax, int ay);

    /*
     * Updates the location using ax/ay
     */
    void childUpdateLocation(int ax, int ay);

    /*
     * Explicitly moves the object - updates location and calls childMove on children.
     */
    void move(int x, int y);

    void setSize(int x, int y);

    void updateLocation();

    bool isHovered();

    void addMessageHandler(IMessageHandler& handler);

    int x{0};
    int y{0};
    int xOffset{0};
    int yOffset{0};
    int xSize{0};
    int ySize{0};

    bool markedForDelete{false};
    BaseMenuObject *parent{ nullptr };

    bool hidden{ false };

protected:
    void emit(Message& msg);

    std::vector<IMessageHandler *> handlers{};
};

}