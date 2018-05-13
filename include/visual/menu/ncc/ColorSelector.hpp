/*
  Created by Jenny White on 12.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <IntegerValueStorage.hpp>
#include "Container.hpp"

namespace zerokernel
{

class ColorSelector: public Container
{
public:
    ColorSelector();

    bool handleSdlEvent(SDL_Event *event) override;

    void render() override;

    void notifyMove() override;

    void handleMessage(Message &msg) override;

    void updateColor();

    IntegerValueStorage red{};
    IntegerValueStorage green{};
    IntegerValueStorage blue{};

    glez::rgba current{};
};

}