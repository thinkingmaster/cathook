/*
  Created by Jenny White on 29.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <SDL2/SDL_events.h>
#include <glez/glez.hpp>
#include "Container.hpp"

class MenuResources
{
    MenuResources();
    ~MenuResources() = default;
    friend MenuResources& resources();
public:
    MenuResources(const MenuResources&) = delete;

    glez::font font_base;
    glez::font font_bold;
    glez::texture tx_cross;
    glez::rgba c_background;
    glez::rgba c_foreground;
    glez::rgba c_foreground_2;
    glez::rgba c_text;
    glez::rgba c_text_outline;
};

MenuResources& resources();

class Menu: public Container
{
public:
    Menu();

    virtual ~Menu() = default;
};