/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "Variable.hpp"
#include "Input.hpp"

class VariableInput: public Variable
{
public:
    inline explicit VariableInput(BaseMenuObject *parent, std::string description): Variable(parent)
    {
        std::unique_ptr<Label> label = std::make_unique<Label>(this, description);
        std::unique_ptr<Input> input = std::make_unique<Input>(this);

        this->label = label.get();
        this->input = input.get();

        input->setOffset(60, 0);
        label->setOffset(0, 2);

        addObject(std::move(label));
        addObject(std::move(input));
    }

    Input *input{ nullptr };
};