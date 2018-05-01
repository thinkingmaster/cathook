/*
  Created by Jenny White on 29.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include "menu.hpp"

#include <ZeroKernelWindow.hpp>
#include <Box.hpp>
#include <Checkbox.hpp>
#include <Variable.hpp>
#include <TabSelection.hpp>
#include <VariableSwitch.hpp>
#include <Input.hpp>
#include <TabContainer.hpp>
#include <VariableInput.hpp>

MenuResources& resources()
{
    static MenuResources object{};
    return object;
}

Menu::Menu(): Container(nullptr)
{
    std::unique_ptr<ZeroKernelWindow> window = std::make_unique<ZeroKernelWindow>(this);
    std::unique_ptr<Box> box = std::make_unique<Box>(window.get());

    window->container->addTab("Test");
    window->container->addTab("Testing");
    window->container->addTab("Testicle");

    auto container = window->container->getTab("Test");

    box->title.setString("Tricks");
    box->setOffset(40, 40);
    box->xSize = 60;
    box->ySize = 30;
    box->list.addObject(std::move(std::make_unique<VariableSwitch>(&box->list, "Testing")));
    box->list.addObject(std::move(std::make_unique<VariableInput>(&box->list, "Testing")));
    box->list.addObject(std::move(std::make_unique<Input>(&box->list)));
    container->addObject(std::move(box));
    addObject(std::move(window));

    move(0, 0);
}

MenuResources::MenuResources()
    : font_base("Verdana.ttf", 10), font_bold("VerdanaBold.ttf", 9), tx_cross("cross.png")
{
    c_background = glez::rgba(29, 47, 64, 255);
    c_foreground = glez::rgba(7, 151, 151, 255);
    c_foreground_2 = glez::rgba(56, 178, 143, 255);
    c_text = glez::rgba(196, 211, 225, 255);
    c_text_outline = glez::rgba(0, 0, 0, 255);
}
