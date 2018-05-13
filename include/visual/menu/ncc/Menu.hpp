/*
  Created by Jenny White on 29.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <SDL2/SDL_events.h>
#include <glez/glez.hpp>
#include "Container.hpp"
#include "Tooltip.hpp"

namespace zerokernel
{

namespace resource::font
{
extern glez::font base;
extern glez::font bold;
}

namespace resource::color
{
extern glez::rgba background;
extern glez::rgba background_tooltip;
extern glez::rgba foreground;
extern glez::rgba foreground_2;
extern glez::rgba text;
extern glez::rgba text_shadow;
}

class Menu
{
public:
    Menu();

    static Menu *instance;

    static void init();
    static void destroy();

    bool handleSdlEvent(SDL_Event *event);
    void render();
    void update();

    void addOverlayObject(std::unique_ptr<BaseMenuObject> &&object);

    void reset();

    void loadFromXml(tinyxml2::XMLElement *element);

    int mouseX;
    int mouseY;

    int dx;
    int dy;

    bool ready{ false };

protected:
    std::unique_ptr<Container> root;
    std::unique_ptr<Container> overlay;
    Tooltip tooltip;

    void showTooltip(std::string tooltip);
};


}