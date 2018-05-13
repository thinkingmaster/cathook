/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <memory>
#include "BaseMenuObject.hpp"

namespace zerokernel
{

class ObjectFactory
{
public:
    static std::unique_ptr<BaseMenuObject> createObjectFromXml(const tinyxml2::XMLElement *element);

};

}