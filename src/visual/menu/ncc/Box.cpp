/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include <ncc/Box.hpp>

namespace zerokernel
{

void Box::setParent(BaseMenuObject *parent)
{
    list.setParent(this);
}

void Box::loadFromXml(const tinyxml2::XMLElement *data)
{
    BaseMenuObject::loadFromXml(data);
    list.loadFromXml(data);
    list.xSize = xSize - 16;
    list.setOffset(8, 9);
    const char *title = "Unnamed";
    data->QueryStringAttribute("name", &title);
    setTitle(title);
}

}