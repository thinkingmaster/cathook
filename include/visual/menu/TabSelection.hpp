/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <vector>
#include <string>
#include "BaseMenuObject.hpp"
#include "menu.hpp"

class TabSelection: public BaseMenuObject
{
public:
    inline explicit TabSelection(BaseMenuObject *parent)
            : BaseMenuObject(parent)
    {
    }

    inline virtual bool handleSdlEvent(SDL_Event *event)
    {
        return BaseMenuObject::handleSdlEvent(event);
    }
    inline virtual void render()
    {
        auto& rx = resources();
        int offset = 0;
        glez::draw::line(x, y + 15, xSize, 0, rx.c_foreground, 1);
        for (int i = 0; i < options.size(); ++i)
        {
            if (i == selected)
            {
                glez::draw::rect(x + offset, y - 1, 14 + widths.at(i), 16, rx.c_foreground);
                glez::draw::line(x + offset + 7, y + 13, widths.at(i), 0, rx.c_background, 1);
            }
            glez::draw::line(x + offsets.at(i), y, 0, 15, rx.c_foreground, 1);
            glez::draw::outlined_string(x + offset + 5, y + 3, options.at(i).c_str(), rx.font_bold, i == selected ? glez::color::white : rx.c_text, rx.c_text_outline, 1, nullptr, nullptr);
            offset = offsets.at(i);
        }
    }
    inline virtual void notifyMove()
    {
        xSize = parent->xSize;
    }

    inline void add(const std::string& option)
    {
        options.push_back(option);
        float width;
        resources().font_base.getStringSize(option, &width, nullptr);
        if (offsets.size())
            offsets.push_back(offsets.at(offsets.size() - 1) + 14 + int(width));
        else
            offsets.push_back(int(width) + 14);
        widths.push_back(int(width));
    }
public:
    std::vector<std::string> options{};
    std::vector<int> offsets{};
    std::vector<int> widths{};
    int selected{ 0 };
};