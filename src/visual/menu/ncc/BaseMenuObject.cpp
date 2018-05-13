/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include <ncc/BaseMenuObject.hpp>
#include <ncc/Menu.hpp>
#include <ncc/Message.hpp>
#include <iostream>

namespace zerokernel
{

std::size_t BaseMenuObject::objectCount{ 0 };

bool BaseMenuObject::handleSdlEvent(SDL_Event *event)
{
    return false;
}

void BaseMenuObject::render()
{
   /* int t = int(this);
    t *= 13333;
    glez::draw::rect(x, y, xSize, ySize, glez::rgba(t & 0xFF, (t >> 8) & 0xFF, (t >> 16) & 0xFF, 70));*/
}

void BaseMenuObject::update()
{

}

void BaseMenuObject::notifyMove()
{

}

void BaseMenuObject::notifySize()
{

}

void BaseMenuObject::setParent(BaseMenuObject *parent)
{
    this->parent = parent;
}

void BaseMenuObject::setOffset(int x, int y)
{
    xOffset = x;
    yOffset = y;
}

void BaseMenuObject::childMove(int x, int y, int ax, int ay)
{
    xOffset = x;
    yOffset = y;
    childUpdateLocation(ax, ay);
}

void BaseMenuObject::childUpdateLocation(int ax, int ay)
{
    x = xOffset + ax;
    y = yOffset + ay;
    notifyMove();
}

void BaseMenuObject::move(int x, int y)
{
    xOffset = x;
    yOffset = y;
    updateLocation();
}

void BaseMenuObject::updateLocation()
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

bool BaseMenuObject::isHovered()
{
    int mx;
    int my;

    if (Menu::instance)
    {
        mx = Menu::instance->mouseX;
        my = Menu::instance->mouseY;
    }

    return mx >= x && mx <= x + xSize && my >= y && my <= y + ySize;
}

void BaseMenuObject::handleMessage(Message &msg)
{
}

void BaseMenuObject::addMessageHandler(IMessageHandler &handler)
{
    handlers.push_back(&handler);
}

void BaseMenuObject::emit(Message &msg)
{
    msg.sender = this;
    for (auto& handler: handlers)
    {
        handler->handleMessage(msg);
    }
}

void BaseMenuObject::loadFromXml(const tinyxml2::XMLElement *data)
{
    std::cout << "Loading object " << data->Name() << "\n";

    data->QueryIntAttribute("x", &xOffset);
    data->QueryIntAttribute("y", &yOffset);
    data->QueryIntAttribute("w", &xSize);
    data->QueryIntAttribute("h", &ySize);
}

bool BaseMenuObject::isHidden()
{
    return hidden;
}

void BaseMenuObject::setSize(int x, int y)
{
    xSize = x;
    ySize = y;
    notifySize();
}

}