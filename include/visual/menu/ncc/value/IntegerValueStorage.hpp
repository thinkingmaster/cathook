/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <ncc/interface/IValueInterface.hpp>

namespace zerokernel
{

class IntegerValueStorage: public IValueInterface
{
public:
    inline std::string toString() override
    {
        return std::to_string(value);
    }
    inline void fromString(std::string string) override
    {
        char *end = nullptr;
        const char *cStr = string.c_str();
        int newValue = std::strtol(cStr, &end, 10);
        if (end == cStr)
        {
            newValue = fallback;
        }
        internalSetValue(newValue);

    }
    inline void increment() override
    {
        int newValue = internalGetValue() + step;
        if (hasMax && newValue > max)
            newValue = max;
        internalSetValue(newValue);

    }

    inline void decrement() override
    {
        int newValue = internalGetValue() - step;
        if (hasMin && newValue < min)
            newValue = min;
        internalSetValue(newValue);

    }

    inline bool hasUpdated() override
    {
        if (lastValue == internalGetValue())
            return false;

        lastValue = internalGetValue();
        return true;
    }

    int max{ 0 };
    int min{ 0 };
    bool hasMin{ false };
    bool hasMax{ false };
    int value{ 0 };
    int lastValue{ 0 };
    int fallback{ 0 };
    int step{ 1 };

protected:
    virtual int internalGetValue()
    {
        return value;
    }

    virtual void internalSetValue(int value)
    {
        this->value = value;
    }
};

}