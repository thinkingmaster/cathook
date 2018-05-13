/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "IValueInterface.hpp"


namespace zerokernel
{

class FloatValueStorage : public IValueInterface
{
public:
    inline std::string toString() override
    {
        return std::to_string(internalGetValue());
    }

    inline void fromString(std::string string) override
    {
        char *end = nullptr;
        const char *cStr = string.c_str();
        float newValue = std::strtof(cStr, &end);
        if (end == cStr)
        {
            newValue = fallback;
        }
        internalSetValue(newValue);

    }

    inline void increment() override
    {
        float newValue = internalGetValue() + step;
        if (hasMax && newValue > max)
            newValue = max;
        internalSetValue(newValue);

    }

    inline void decrement() override
    {
        float newValue = internalGetValue() - step;
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

    float max{0};
    float min{0};
    bool hasMin{false};
    bool hasMax{false};
    float value{0};
    float lastValue{0};
    float fallback{0};
    float step{1};

protected:
    virtual float internalGetValue()
    {
        return value;
    }

    virtual void internalSetValue(float value)
    {
        this->value = value;
    }
};

}