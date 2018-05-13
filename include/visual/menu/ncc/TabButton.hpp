/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "BaseMenuObject.hpp"
#include "TabContainer.hpp"
#include "Button.hpp"
#include <string>
#include <glez/glez.hpp>

namespace zerokernel
{

class TabSelection;

class TabButton: public Button
{
public:
    explicit TabButton(TabSelection *parent, int id);

    virtual void render() override;
    int id;
protected:
    virtual void onButtonClick() override;
};

}