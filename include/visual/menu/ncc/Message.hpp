/*
  Created by Jenny White on 13.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <string>
#include <boost/any.hpp>
#include <unordered_map>
#include "BaseMenuObject.hpp"

namespace zerokernel
{

class Message
{
public:
    Message(std::string name);

    bool has(std::string key);
    void set(std::string key, boost::any value);

    std::string name{};
    BaseMenuObject *sender{ nullptr };
    std::unordered_map<std::string, boost::any> values{};
};

}