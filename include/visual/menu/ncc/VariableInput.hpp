/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "Variable.hpp"
#include "Input.hpp"


namespace zerokernel
{

class VariableInput : public Variable
{
public:
    inline explicit VariableInput(std::string description,
                                  std::shared_ptr<IValueInterface> bridge)
            : Variable{}
    {
        std::unique_ptr<Label> label = std::make_unique<Label>(description);
        std::unique_ptr<Input> input = std::make_unique<Input>(bridge.get());

        this->label = label.get();
        this->input = input.get();

        addObject(std::move(label));
        addObject(std::move(input));
    }

    void notifyMove() override
    {
        Variable::notifyMove();
        label->childMove(0, 2, x, y);
        input->childMove(xSize - input->xSize, 0, x, y);
    }

    Input *input{nullptr};
};

}