/*
  Created by Jenny White on 29.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "Container.hpp"
#include "List.hpp"
#include "Menu.hpp"
#include <glez/glez.hpp>

namespace zerokernel
{

class Box : public BaseMenuObject
{
public:
    inline explicit Box() : BaseMenuObject{}
    {
        list.setParent(this);
        list.setOffset(8, 10);
    }

    inline virtual void render()
    {
        glez::draw::rect_outline(x + 2, y + 5, xSize - 4, ySize - 7, resource::color::foreground, 1);
        glez::draw::rect(x + (xSize - titleWidth) / 2 - 3, y, titleWidth + 6, 10, resource::color::background);
        glez::draw::outlined_string(x + (xSize - titleWidth) / 2, y, title, resource::font::bold, resource::color::text, resource::color::text_shadow, 1, nullptr, nullptr);

        list.render();

        BaseMenuObject::render();
    }

    virtual bool handleSdlEvent(SDL_Event *event)
    {
        return list.handleSdlEvent(event);
    }

    virtual void update() override
    {
        list.update();
    }

    virtual void notifySize() override
    {
        list.notifySize();
    }

    virtual void notifyMove()
    {
        list.childUpdateLocation(x, y);
        ySize = list.ySize + 16;
    }

    void loadFromXml(const tinyxml2::XMLElement *data) override;

    void setParent(BaseMenuObject *parent) override;

    void setTitle(std::string title)
    {
        this->title = title;
        float w;
        resource::font::bold.getStringSize(title, &w, nullptr);
        titleWidth = int(w);
    }

    std::string title;
    int titleWidth;

    List list{};
};

}