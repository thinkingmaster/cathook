/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <string>

namespace zerokernel
{

class IValueInterface
{
public:
    virtual std::string toString() = 0;

    virtual void fromString(std::string string) = 0;

    virtual void increment() = 0;

    virtual void decrement() = 0;

    virtual bool hasUpdated() = 0;
};

}