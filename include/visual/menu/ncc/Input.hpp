/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "BaseMenuObject.hpp"
#include "Menu.hpp"
#include "IValueInterface.hpp"
#include "Message.hpp"
#include <glez/glez.hpp>
#include <functional>


namespace zerokernel
{

class Input : public BaseMenuObject
{
public:
    inline explicit Input(IValueInterface *value) : BaseMenuObject{}, value(value)
    {
        xSize = 30;
        ySize = 14;
        acceptValue();
    }

    inline virtual bool handleSdlEvent(SDL_Event *event)
    {
        switch (event->type)
        {
        case SDL_MOUSEWHEEL:
            if (isHovered())
            {
                if (event->wheel.y > 0)
                    value->increment();
                else
                    value->decrement();
                Message msg{ "ValueChanged" };
                emit(msg);
                return true;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
        {
            if (isHovered())
            {
                if (clicked)
                {
                    if (!isCapturingKeys)
                    {
                        editStart();
                    }
                }
                clicked = true;
                return true;
            } else
            {
                if (isCapturingKeys)
                {
                    discardValue();
                    return true;
                }
                if (clicked)
                    clicked = false;
            }
            return false;
        }
        case SDL_TEXTINPUT:
            if (isCapturingKeys)
                updateText(string + event->text.text);
            return true;
        case SDL_KEYDOWN:
            if (isCapturingKeys)
            {
                switch (event->key.keysym.sym)
                {
                case SDLK_BACKSPACE:
                    if (string.size())
                    {
                        string.pop_back();
                    }
                    return true;
                case SDLK_RETURN:
                    acceptValue();
                    return true;
                case SDLK_ESCAPE:
                    discardValue();
                    return true;
                default:
                    break;
                }
            }
        default:
            break;
        }

        return false;
    }

    inline virtual void render()
    {
        if (!isCapturingKeys)
        {
            if (value->hasUpdated())
                updateText(value->toString());
        }

        if (propDrawBorder)
            glez::draw::rect_outline(x, y, xSize, ySize,
                                 isCapturingKeys ? resource::color::foreground_2
                                                 : resource::color::foreground, 1);
        utility::drawCenteredString(x + xSize / 2, y + 3, string, resource::font::bold, resource::color::text);

        BaseMenuObject::render();
    }

    void editStart()
    {
        lastValue = string;
        isCapturingKeys = true;
    }

    void editFinish()
    {
        isCapturingKeys = false;
    }

    void acceptValue()
    {
        value->fromString(string);
        updateText(value->toString());
        editFinish();
        Message msg{ "ValueChanged" };
        emit(msg);
    }

    void discardValue()
    {
        updateText(lastValue);
        editFinish();
    }

    void updateText(std::string text)
    {
        string = std::move(text);
    }

    bool isCapturingKeys{false};
    bool clicked{false};

    std::string string{};
    std::string lastValue{};

    IValueInterface *value;

    // Properties

    bool propDrawBorder{ true };
};

}