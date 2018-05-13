/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include <ObjectFactory.hpp>
#include <Box.hpp>
#include <TabContainer.hpp>
#include <Window.hpp>
#include <iostream>
#include <Checkbox.hpp>
#include <ColorSelector.hpp>

namespace zerokernel
{

std::unique_ptr<BaseMenuObject>
ObjectFactory::createObjectFromXml(const tinyxml2::XMLElement *element)
{
    std::cout << "Loading an element\n";

    std::unique_ptr<BaseMenuObject> result{ nullptr };
    std::string type = element->Name();

    if (type == "Window")
        result = std::make_unique<Window>();
    else if (type == "TabContainer")
        result = std::make_unique<TabContainer>();
    else if (type == "Box")
        result = std::make_unique<Box>();
    else if (type == "Checkbox")
        result = std::make_unique<Checkbox>();
    else if (type == "ColorSelector")
        result = std::make_unique<ColorSelector>();

    if (result.get() != nullptr)
    {
        result->loadFromXml(element);
        return result;
    }

    std::cout << "FATAL: Unknown object type: " << type << "\n";
    return nullptr;
}
}