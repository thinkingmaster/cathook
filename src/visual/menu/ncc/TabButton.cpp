/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include "ncc/TabButton.hpp"

namespace zerokernel
{

TabButton::TabButton(TabSelection *parent, int id) : Button{}, id(id)
{
    setParent(parent);
    ySize = 14;
    float width;
    resource::font::bold.getStringSize(parent->options.at(id), &width,
                                        nullptr);
    xSize = int(width) + 12;
}

void TabButton::render()
{
    bool selected = ((TabSelection *) parent)->selected == id;
    if (selected)
    {
        glez::draw::rect(x, y, xSize, ySize, resource::color::foreground);
        glez::draw::line(x + 6, y + ySize - 3, xSize - 12, 0,
                         resource::color::background, 1);
    } else if (isHovered())
        glez::draw::rect(x, y, xSize, ySize, resource::color::foreground_2);
    glez::draw::rect_outline(x, y, xSize, ySize, resource::color::foreground, 1);
    glez::draw::outlined_string(x + 6, y + 2, ((TabSelection *) parent)->options.at(id), resource::font::bold, selected ? glez::color::white : resource::color::text, resource::color::text_shadow, 1, nullptr, nullptr);

    BaseMenuObject::render();
}

void TabButton::onButtonClick()
{
    ((TabSelection *) parent)->selected = id;
    Button::onButtonClick();
}

}