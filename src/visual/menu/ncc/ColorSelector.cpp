/*
  Created by Jenny White on 12.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include <ncc/ColorSelector.hpp>
#include <ncc/Input.hpp>
#include <ncc/interface/IValueInterface.hpp>

static void internalValueChangeHandler(const zerokernel::IValueInterface *interface)
{

}

namespace zerokernel
{

ColorSelector::ColorSelector(): Container()
{
    red.fromString("0");
    green.fromString("0");
    blue.fromString("0");
    red.max = 255;
    red.hasMax = true;
    green.max = 255;
    green.hasMax = true;
    blue.max = 255;
    blue.hasMax = true;
    red.min = 0;
    red.hasMin = true;
    green.min = 0;
    green.hasMin = true;
    blue.min = 0;
    blue.hasMin = true;

    std::unique_ptr<Input> inputRed = std::make_unique<Input>(&red);
    std::unique_ptr<Input> inputGreen = std::make_unique<Input>(&green);
    std::unique_ptr<Input> inputBlue = std::make_unique<Input>(&blue);

    inputRed->addMessageHandler(*this);
    inputGreen->addMessageHandler(*this);
    inputBlue->addMessageHandler(*this);

    inputRed->setOffset(0, 0);
    inputGreen->setOffset(25, 0);
    inputBlue->setOffset(50, 0);
    xSize = 75;
    ySize = 16;
    inputRed->xSize = 25;
    inputGreen->xSize = 25;
    inputBlue->xSize = 25;
    inputRed->propDrawBorder = false;
    inputGreen->propDrawBorder = false;
    inputBlue->propDrawBorder = false;

    addObject(std::move(inputRed));
    addObject(std::move(inputGreen));
    addObject(std::move(inputBlue));
}

bool ColorSelector::handleSdlEvent(SDL_Event *event)
{
    return Container::handleSdlEvent(event);
}

void ColorSelector::render()
{
    glez::draw::rect(x, y, xSize, ySize, current);
    Container::render();
}

void ColorSelector::notifyMove()
{
    Container::notifyMove();
}

void ColorSelector::handleMessage(Message &msg)
{
    if (msg.name == "ValueChanged")
    {
        updateColor();
        Message msg{ "ValueChanged" };
        emit(msg);
    }
}

void ColorSelector::updateColor()
{
    current = glez::rgba(red.value, green.value, blue.value);
}

}