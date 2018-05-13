/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "IValueInterface.hpp"

namespace zerokernel
{

class StringValueStorage : public IValueInterface
{
public:
    inline std::string toString() override
    {
        return internalGetValue();
    }

    inline void fromString(std::string string) override
    {
        if (hasMax && string.size() > max)
        {
            string = string.substr(0, max);
        }
        internalSetValue(string);

    }

    inline void increment() override
    {
    }

    inline void decrement() override
    {
    }

    inline bool hasUpdated() override
    {
        if (internalGetValue() == lastValue)
            return false;
        lastValue = internalGetValue();
        return true;
    }

    std::string value{};
    std::string lastValue{};
    unsigned max{0};
    bool hasMax{false};

protected:
    virtual std::string internalGetValue()
    {
        return value;
    }

    virtual void internalSetValue(std::string value)
    {
        this->value = std::move(value);
    }
};

}