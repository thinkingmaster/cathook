/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "BaseMenuObject.hpp"
#include "Utility.hpp"
#include <string>

namespace zerokernel
{

class Tooltip: public BaseMenuObject
{
public:
    Tooltip();

    virtual void render();

    void setText(std::string text);

    std::string text;
    std::string lastText;
    bool shown{false};
};

}