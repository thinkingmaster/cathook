/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "BaseMenuObject.hpp"
#include "Container.hpp"
#include "TabSelection.hpp"

namespace zerokernel
{

class TabContainer: public BaseMenuObject
{
public:
    inline explicit TabContainer(): BaseMenuObject{}, selection(this)
    {
    }

    inline virtual bool handleSdlEvent(SDL_Event *event) override
    {
        selection.handleSdlEvent(event);
        if (containers.size())
            containers.at(selection.selected)->handleSdlEvent(event);
    }

    inline virtual void render()
    {
        selection.render();
        if (containers.size())
            containers.at(selection.selected)->render();

        BaseMenuObject::render();
    }

    inline virtual void update()
    {
        selection.update();
        if (containers.size())
            containers.at(selection.selected)->update();
    }

    inline virtual void notifyMove()
    {
        selection.childUpdateLocation(x, y);
        for (auto& c: containers)
        {
            c->childUpdateLocation(x, y);
        }
    }

    void notifySize() override;

    void loadFromXml(const tinyxml2::XMLElement *data) override;

    inline void addTab(std::string title)
    {
        selection.add(title);
        containers.push_back(std::move(std::make_unique<Container>()));
        containers.at(containers.size() - 1)->childMove(0, selection.ySize, x, y);
        containers.at(containers.size() - 1)->setParent(this);
    }

    inline Container *getTab(std::string title)
    {
        for (int i = 0; i < selection.options.size(); ++i)
        {
            if (selection.options.at(i) == title)
            {
                return containers.at(i).get();
            }
        }
        return nullptr;
    }

    TabSelection selection;
    std::vector<std::unique_ptr<Container>> containers{};
};

}