/*
  Created by Jenny White on 29.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "Container.hpp"
#include "menu.hpp"

class Window: public Container
{
public:
    inline explicit Window(BaseMenuObject *parent): Container(parent)
    {
    }

    inline virtual bool handleSdlEvent(SDL_Event *event)
    {
        return Container::handleSdlEvent(event);
    }
    inline virtual void render()
    {
        auto& rx = resources();

        glez::draw::rect(x, y, xSize, ySize, rx.c_background);
        glez::draw::rect_outline(x, y, xSize, ySize, rx.c_foreground, 1.0f);
        glez::draw::outlined_string(x + titleX, y + 4, title.c_str(), rx.font_bold, glez::color::white, rx.c_text_outline, 1.25f, nullptr, nullptr);
        glez::draw::line(x, y + 15, xSize, 0, rx.c_foreground, 1.0f);

        Container::render();
    }

    inline void setTitle(const std::string& title)
    {
        this->title = title;
        updateTitleLocation();
    }

    inline void updateTitleLocation()
    {
        float x;
        resources().font_bold.getStringSize(title, &x, nullptr);
        titleX = (xSize - int(x)) / 2;
    }

public:
    std::string title{ "Unnamed" };
    int titleX{ 0 };
};