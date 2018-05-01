/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "BaseMenuObject.hpp"
#include "menu.hpp"
#include <string>

class Label: public BaseMenuObject
{
public:
    inline explicit Label(BaseMenuObject *parent, std::string contents): BaseMenuObject(parent)
    {
        setContents(contents);
    }

    virtual void render()
    {
        auto& rx = resources();
        glez::draw::outlined_string(x, y, contents, rx.font_base, rx.c_text, rx.c_text_outline, 1.25, nullptr, nullptr);
    }

    void setContents(std::string contents)
    {
        this->contents = std::move(contents);

        float width, height;
        resources().font_base.getStringSize(contents, &width, &height);
        xSize = int(width);
        ySize = int(height);
    }

    std::string contents{};
};