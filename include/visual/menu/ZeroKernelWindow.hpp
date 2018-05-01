/*
  Created by Jenny White on 29.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "Window.hpp"
#include "TabContainer.hpp"

/**
 * A specialized main window
 */
class ZeroKernelWindow: public Window
{
public:
    inline explicit ZeroKernelWindow(BaseMenuObject *parent): Window(parent)
    {
        x = 0;
        y = 0;
        xSize = 300;
        ySize = 300;
        setTitle("Zero Kernel");

        std::unique_ptr<TabContainer> tabContainer = std::make_unique<TabContainer>(this);
        container = tabContainer.get();
        container->setOffset(0, 16);
        addObject(std::move(tabContainer));
    }

    TabContainer *container;
};