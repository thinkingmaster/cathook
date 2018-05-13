/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include <ncc/TabContainer.hpp>

namespace zerokernel
{

void TabContainer::loadFromXml(const tinyxml2::XMLElement *data)
{
    BaseMenuObject::loadFromXml(data);

    auto el = data->FirstChildElement("Tab");
    while (el != nullptr)
    {
        const char *name = "Unnamed";
        el->QueryStringAttribute("name", &name);
        addTab(name);
        containers.at(containers.size() - 1)->loadFromXml(el);
        el = el->NextSiblingElement("Tab");
    }
}

void TabContainer::notifySize()
{
    BaseMenuObject::notifySize();

    selection.notifySize();
    for (auto& c: containers)
    {
        c->notifySize();
    }
}
}