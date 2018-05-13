/*
  Created by Jenny White on 30.04.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "BaseMenuObject.hpp"
#include "Checkbox.hpp"
#include "Variable.hpp"
#include "Label.hpp"

#include <string>
#include <functional>

namespace zerokernel
{

class VariableSwitch: public Variable
{
public:
    inline explicit VariableSwitch(std::string description): Variable{}
    {
        std::unique_ptr<Label> label = std::make_unique<Label>(description);
        std::unique_ptr<Checkbox> checkbox = std::make_unique<Checkbox>();

        this->label = label.get();
        this->checkbox = checkbox.get();

        checkbox->setOffset(0, 0);
        label->setOffset(18, 2);

        addObject(std::move(label));
        addObject(std::move(checkbox));
    }

    Checkbox *checkbox{ nullptr };

    std::function<void(VariableSwitch *)> onSwitchToggled{ nullptr };
};

}