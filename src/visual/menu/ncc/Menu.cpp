/*
  Created by Jenny White on 29.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include "ncc/Menu.hpp"

#include <ncc/Box.hpp>
#include <ncc/Checkbox.hpp>
#include <ncc/Variable.hpp>
#include <ncc/TabSelection.hpp>
#include <ncc/VariableSwitch.hpp>
#include <ncc/Input.hpp>
#include <ncc/TabContainer.hpp>
#include <ncc/VariableInput.hpp>
#include <ncc/VariableFactory.hpp>
#include <ncc/InputButton.hpp>
#include <ncc/Combobox.hpp>
#include <ncc/Slider.hpp>
#include <ncc/Window.hpp>
#include <fstream>
#include <iostream>
#include <core/cvwrapper.hpp>
#include <visual/drawing.hpp>

static CatCommand reload("menu_ncc_reload", "Reload the menu", [](const CCommand& args) {
    tinyxml2::XMLDocument doc{};
    doc.LoadFile(DATA_PATH "/menu.xml");
    zerokernel::Menu::instance->loadFromXml(doc.RootElement());
});

namespace zerokernel
{

Menu *Menu::instance{ nullptr };

namespace resource::font
{
glez::font base{ "Verdana.ttf", 10 };
glez::font bold{ "VerdanaBold.ttf", 9 };
}

namespace resource::color
{
glez::rgba background = glez::rgba(29, 47, 64);
glez::rgba background_tooltip = glez::rgba(0, 0, 0, 100);
glez::rgba foreground = glez::rgba(7, 151, 151);
glez::rgba foreground_2 = glez::rgba(56, 178, 143);
glez::rgba text = glez::rgba(196, 211, 225);
glez::rgba text_shadow = glez::rgba(0, 0, 0);
}

Menu::Menu() : overlay{ nullptr }, tooltip{}, root{ nullptr }
{
    tinyxml2::XMLDocument doc{};
    doc.LoadFile(DATA_PATH "/menu.xml");
    loadFromXml(doc.RootElement());

/*    std::unique_ptr<Window> window = std::make_unique<Window>();
    std::unique_ptr<Box> box = std::make_unique<Box>();
    std::unique_ptr<InputButton> button = std::make_unique<InputButton>();
    std::unique_ptr<TabContainer> container = std::make_unique<TabContainer>();

    window->xSize = 400;
    window->ySize = 300;

    container->setParent(window.get());
    window->setTitle("Zero Kernel");
    container->setOffset(0, 15);
    container->addTab("Test");
    container->addTab("Testing");
    container->addTab("Testicle");

    auto ccontainer = container->getTab("Test");

    box->setTitle("Tricks");
    box->setOffset(1, 40);
    box->xSize = 60;
    box->ySize = 30;
    box->list.addObject(
            std::move(std::make_unique<VariableSwitch>("Testing")));
    box->list.addObject(std::move(VariableFactory::createInteger()));
    box->list.addObject(std::move(VariableFactory::createInteger()));
    box->list.addObject(std::move(button));
    //box->update();

    std::unique_ptr<Combobox> cbox = std::make_unique<Combobox>();
    cbox->addOption("Testing");
    cbox->addOption("Testing2");
    cbox->addOption("Testing4");
    cbox->addOption("Test");

    auto fovbox = std::make_unique<Box>();
    fovbox->setTitle("FOV");
    fovbox->setOffset(250, 10);
    fovbox->list.addObject(std::move(std::make_unique<Slider>()));

    box->list.addObject(std::move(cbox));
    ccontainer->addObject(std::move(box));
    ccontainer->addObject(std::move(fovbox));
    window->addObject(std::move(container));
    root->addObject(std::move(window));

    root->move(0, 0);*/
}

void Menu::showTooltip(std::string tooltip)
{
    if (!ready)
        return;
    this->tooltip.shown = true;
    this->tooltip.setText(tooltip);
}

void Menu::addOverlayObject(std::unique_ptr<BaseMenuObject> &&object)
{
    if (!ready)
        return;
    overlay->addObject(std::move(object));
}

bool Menu::handleSdlEvent(SDL_Event *event)
{
    if (!ready)
        return false;
    return overlay->handleSdlEvent(event) || root->handleSdlEvent(event);
}

void Menu::render()
{
    if (!ready)
        return;
    root->render();
    overlay->render();
    if (tooltip.shown)
        tooltip.render();
}

void Menu::update()
{
    if (!ready)
        return;
    int mx, my;
    SDL_GetMouseState(&mx, &my);
    dx = mx - mouseX;
    dy = my - mouseY;
    mouseX = mx;
    mouseY = my;

    root->update();
    overlay->update();
    if (tooltip.shown)
        tooltip.update();
}

void Menu::init()
{
    instance = new Menu();
}

void Menu::destroy()
{
    delete instance;
}

void Menu::reset()
{
    root = std::make_unique<Container>();
    overlay = std::make_unique<Container>();
    ready = true;
}

void Menu::loadFromXml(tinyxml2::XMLElement *element)
{
    reset();
    root->setOffset(0, 0);
    root->xSize = draw::width;
    root->ySize = draw::height;
    root->loadFromXml(element);
    root->move(0, 0);
    root->notifySize();
}

}