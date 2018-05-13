/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include <glez/font.hpp>
#include <sstream>
#include <glez/color.hpp>
#include <glez/draw.hpp>
#include <ncc/Menu.hpp>

namespace utility
{


std::string wrapString(const std::string& input, glez::font& font, int width, int *resultWidth, int *resultLines)
{
    int lineWidth{ 0 };
    float wordWidth{ 0 };
    int isspace{ false };
    std::ostringstream resultStream{};
    std::ostringstream wordStream{};
    std::istringstream inputStream{ input };
    int lineCount{ 1 };
    int maxWidth{ 0 };

    bool wrapped{ true };

    auto splitWord = [&](bool newln) {
        std::string word = wordStream.str();
        wordStream.str("");
        if (not word.empty())
        {
            font.getStringSize((lineWidth == 0) ? word : ' ' + word, &wordWidth, nullptr);
            if (wordWidth + lineWidth > width && !newln)
            {
                resultStream << '\n';
                lineCount++;
                if (lineWidth > maxWidth)
                    maxWidth = lineWidth;
                lineWidth = 0;
            }
            if (lineWidth)
                resultStream << ' ';
            resultStream << word;
            lineWidth += wordWidth;
        }
    };

    for (auto& c: input)
    {
        if (c == ' ')
        {
            splitWord(false);
        }
        else if (c == '\n')
        {
            splitWord(true);
            resultStream << '\n';
            lineCount++;
            if (lineWidth > maxWidth)
                maxWidth = lineWidth;
            lineWidth = 0;
        }
        else
            wordStream << c;
    }

    splitWord(false);

    if (resultLines)
        *resultLines = lineCount;
    if (resultWidth)
        *resultWidth = maxWidth;

    return resultStream.str();
}

std::string dotCompactString(const std::string& input, glez::font& font, int width, bool reverse)
{
    float dotsWidth;
    unsigned charCount = 0;
    bool compact = false;
    font.getStringSize("...", &dotsWidth, nullptr);

    for (auto i = 1u; i <= input.size(); ++i)
    {
        float tempWidth;
        font.getStringSize(reverse ? input.substr(input.size() - 1 - i, i) : input.substr(0, i), &tempWidth, nullptr);
        if (tempWidth + dotsWidth < width)
            charCount = i;
        else
        {
            if (tempWidth > width)
            {
                compact = true;
                break;
            }
        }
    }

    if (!compact)
        return input;
    if (reverse)
        return "..." + input.substr(input.length() - 1 - charCount, charCount);
    else
        return input.substr(0, charCount) + "...";
}

void drawCenteredString(int x, int y, const std::string &string,
                                 glez::font &font, glez::rgba color)
{
    float width;
    font.getStringSize(string, &width, nullptr);
    glez::draw::outlined_string(x - width / 2, y, string, font, color, zerokernel::resource::color::text_shadow, 1, nullptr, nullptr);
}

}