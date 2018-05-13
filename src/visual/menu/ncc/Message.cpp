/*
  Created by Jenny White on 13.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include <ncc/Message.hpp>

namespace zerokernel
{

Message::Message(std::string name)
    : name(name)
{
}

bool Message::has(std::string key)
{
    return values.find(key) != values.end();
}

void Message::set(std::string key, boost::any value)
{
    values[key] = value;
}

}