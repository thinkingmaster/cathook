/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include "Combobox.hpp"
#include <glez/glez.hpp>
#include <ComboboxList.hpp>

namespace zerokernel
{

Combobox::Combobox(): BaseMenuObject{}
{
    xSize = 100;
    ySize = 14;
}

bool Combobox::handleSdlEvent(SDL_Event *event)
{
    if (!isHovered())
        return false;

    switch (event->type)
    {
    case SDL_MOUSEWHEEL:
    {
        if (event->wheel.direction > 0)
        {
            if (selected < options.size() - 1)
                ++selected;
        } else
        {
            if (selected > 0)
                --selected;
        }
        return true;
    }
    case SDL_MOUSEBUTTONDOWN:
    {
        openOverlay();
        return true;
    }
    default:
        return false;
    }
}

void Combobox::render()
{
    glez::draw::rect_outline(x, y, xSize, ySize, resource::color::foreground, 1);
    if (selected < options.size() && !overlayOpen)
        utility::drawCenteredString(x + xSize / 2, y + 2, options.at(selected),
                                    resource::font::bold, resource::color::text);

    BaseMenuObject::render();
}

void Combobox::addOption(std::string name)
{
    options.push_back(name);
}

void Combobox::openOverlay()
{
    std::unique_ptr<ComboboxList> list = std::make_unique<ComboboxList>(this);
    list->xSize = xSize;
    list->childMove(x, y, 0, 0);
    Menu::instance->addOverlayObject(std::move(list));
    overlayOpen = true;
}

void Combobox::select(std::size_t sel)
{
    selected = sel;
}

void Combobox::onOverlayClose()
{
    overlayOpen = false;
}

}
