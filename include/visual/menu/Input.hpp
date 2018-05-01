/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "BaseMenuObject.hpp"
#include "menu.hpp"
#include <glez/glez.hpp>

class Input: public BaseMenuObject
{
public:
    inline Input(BaseMenuObject *parent): BaseMenuObject(parent)
    {
        xSize = 30;
        ySize = 14;
        updateText("Unkn");
    }

    inline virtual bool handleSdlEvent(SDL_Event *event)
    {
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        bool bounds = isInBounds(mx, my);

        switch (event->type)
        {
        case SDL_MOUSEBUTTONDOWN:
            if (bounds)
            {
                if (clicked)
                    isCapturingKeys = !isCapturingKeys;
                clicked = true;
            }
            else
            {
                isCapturingKeys = false;
                clicked = false;
            }
            return true;
        case SDL_TEXTINPUT:
            if (isCapturingKeys)
                updateText(string + event->text.text);
            break;
        case SDL_KEYDOWN:
            if (isCapturingKeys)
            {
                if (event->key.keysym.sym == SDLK_BACKSPACE)
                {
                    string.pop_back();
                    updateWidth();
                }
            }
        }

        return false;
    }

    inline virtual void render()
    {
        auto& rx = resources();
        glez::draw::rect_outline(x, y, xSize, ySize, isCapturingKeys ? rx.c_foreground_2 : rx.c_foreground, 1);
        glez::draw::outlined_string(x + (xSize - stringWidth) / 2, y + 3, string, rx.font_bold, rx.c_text, rx.c_text_outline, 1, nullptr, nullptr);
    }

    void updateText(std::string text)
    {
        string = std::move(text);
        updateWidth();
    }

    void updateWidth()
    {
        float width;
        resources().font_bold.getStringSize(string, &width, nullptr);
        stringWidth = int(width);
    }

    bool isCapturingKeys{ false };
    // Timer lastClicked{};
    bool clicked{ false };
    std::string string{};
    int stringWidth{ 0 };
};