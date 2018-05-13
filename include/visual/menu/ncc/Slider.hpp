/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "BaseMenuObject.hpp"

namespace zerokernel
{

class Slider : public BaseMenuObject
{
public:
    Slider();

    virtual bool handleSdlEvent(SDL_Event *event) override;

    virtual void render() override;

    virtual void update() override;

    void setValue(int value);

    bool grabbed{false};
    int sliderLocation{0};
};

}