/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "BaseMenuObject.hpp"
#include <string>
#include <vector>


namespace zerokernel
{

class Combobox : public BaseMenuObject
{
public:
    Combobox();

    virtual bool handleSdlEvent(SDL_Event *event) override;

    virtual void render() override;

    void addOption(std::string name);

    void openOverlay();
    void onOverlayClose();

    void select(std::size_t sel);

    std::size_t selected{0};
    std::vector<std::string> options{};
    bool overlayOpen{ false };
};

}