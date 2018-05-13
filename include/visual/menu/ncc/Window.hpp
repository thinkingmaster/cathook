/*
  Created by Jenny White on 29.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "Container.hpp"
#include "Menu.hpp"


namespace zerokernel
{

class Window : public Container
{
public:
    virtual bool handleSdlEvent(SDL_Event *event);

    inline virtual void render()
    {
        glez::draw::rect(x, y, xSize, ySize, resource::color::background);
        glez::draw::rect_outline(x, y, xSize, ySize, resource::color::foreground, 1.0f);
        utility::drawCenteredString(x + xSize / 2, y + 4, title, resource::font::bold, glez::color::white);
        glez::draw::line(x, y + 15, xSize, 0, resource::color::foreground, 1.0f);

        Container::render();
    }

    void loadFromXml(const tinyxml2::XMLElement *data) override;

    void update() override;

    inline void setTitle(const std::string &title)
    {
        this->title = title;
    }

public:
    bool dragged{ false };
    std::string title;
};

}