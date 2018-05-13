/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include <ncc/Container.hpp>
#include <iostream>
#include <ncc/ObjectFactory.hpp>

namespace zerokernel
{

void Container::loadFromXml(const tinyxml2::XMLElement *data)
{
    BaseMenuObject::loadFromXml(data);

    auto first = data->FirstChild();
    if (first == nullptr || first->ToElement() == nullptr)
        return;

    auto el = first->ToElement();
    while (el != nullptr)
    {
        addObject(ObjectFactory::createObjectFromXml(el));

        auto nextNode = el->NextSibling();
        if (nextNode == nullptr)
            break;
        el = nextNode->ToElement();
    }
}
}