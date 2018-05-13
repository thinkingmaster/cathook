/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "BaseMenuObject.hpp"
#include "Menu.hpp"
#include <string>


namespace zerokernel
{

class Label: public BaseMenuObject
{
public:
    inline explicit Label(std::string contents): BaseMenuObject{}
    {
        setContents(contents);
    }

    virtual void render()
    {
        glez::draw::outlined_string(x, y, contents, resource::font::base, resource::color::text,
                                    resource::color::text_shadow, 1.25, nullptr, nullptr);

        BaseMenuObject::render();
    }

    void setContents(std::string contents)
    {
        this->contents = std::move(contents);

        float width, height;
        resource::font::base.getStringSize(contents, &width, &height);
        xSize = int(width);
        ySize = int(height);
    }

    std::string contents{};
};

}