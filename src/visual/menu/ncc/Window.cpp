/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include <Window.hpp>
#include <Menu.hpp>

namespace zerokernel
{

bool Window::handleSdlEvent(SDL_Event *event)
{
    if (event->type == SDL_MOUSEBUTTONDOWN && (Menu::instance->mouseX >= x && Menu::instance->mouseX <= x + xSize && Menu::instance->mouseY >= y && Menu::instance->mouseY <= y + 15))
        dragged = true;
    else if (event->type == SDL_MOUSEBUTTONUP && isHovered())
        dragged = false;
    return Container::handleSdlEvent(event);
}

void Window::update()
{
    if (dragged)
        move(x + Menu::instance->dx, y + Menu::instance->dy);
    Container::update();
}

void Window::loadFromXml(const tinyxml2::XMLElement *data)
{
    Container::loadFromXml(data);
    const char *title = "Unnamed";
    data->QueryStringAttribute("name", &title);
    setTitle(title);
}

}