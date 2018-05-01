/*
  Created by Jenny White on 29.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "BaseMenuObject.hpp"
#include "menu.hpp"

#include <string>
#include <glez/draw.hpp>

class CenteredString: public BaseMenuObject
{
public:
    inline explicit CenteredString(BaseMenuObject *parent)
            : BaseMenuObject(parent)
    {
    }

    inline virtual void render()
    {
        auto& rx = resources();
        glez::draw::outlined_string(x + (xSize - xSizeReal) / 2, y, string, rx.font_bold, rx.c_text, rx.c_text_outline, 1.25f, nullptr, nullptr);
    }

    inline void setString(const std::string& string)
    {
        this->string = string;
        updateRealSize();
    }
    inline void updateRealSize()
    {
        float x;
        resources().font_bold.getStringSize(string, &x, nullptr);
        xSizeReal = int(x);
    }

    int xSizeReal;
    std::string string;
};