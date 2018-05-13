/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include <vector>
#include <string>
#include "BaseMenuObject.hpp"
#include "Menu.hpp"

namespace zerokernel
{

class TabContainer;

class TabSelection : public Container
{
public:
    explicit TabSelection(TabContainer *parent);

    void render() override;

    void notifySize() override;

    void add(const std::string &option);

public:
    std::vector<std::string> options{};
    int offset{0};
    int selected{0};
};

}