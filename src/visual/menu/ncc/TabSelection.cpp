/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include "ncc/TabSelection.hpp"
#include "ncc/TabButton.hpp"

namespace zerokernel
{

void TabSelection::render()
{
    Container::render();
    glez::draw::line(x, y + ySize - 1, xSize, 0, resource::color::foreground, 1);
}

void TabSelection::add(const std::string &option)
{
    options.push_back(option);
    std::unique_ptr<TabButton> button = std::make_unique<TabButton>(this,
                                                                    int(options.size() -
                                                                        1));
    button->childMove(offset, 0, x, y);
    offset += button->xSize - 1;
    addObject(std::move(button));
    /*float width;
    resources().font_base.getStringSize(option, &width, nullptr);
    if (offsets.size())
        offsets.push_back(offsets.at(offsets.size() - 1) + 14 + int(width));
    else
        offsets.push_back(int(width) + 14);
    widths.push_back(int(width));*/
}

TabSelection::TabSelection(TabContainer *parent)
        : Container{}
{
    ySize = 14;
    setParent(parent);
}

void TabSelection::notifySize()
{
    xSize = parent->xSize;
    Container::notifySize();
}

}