/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "Variable.hpp"
#include "VariableInput.hpp"
#include "VariableSwitch.hpp"
#include "IntegerValueStorage.hpp"
#include <memory>


namespace zerokernel
{

class VariableFactory
{
public:
    inline static std::unique_ptr<Variable> createInteger()
    {
        std::shared_ptr<IntegerValueStorage> value = std::make_shared<IntegerValueStorage>();
        std::unique_ptr<VariableInput> result = std::make_unique<VariableInput>("Thinking", value);
        return result;
    }

    /*inline static std::unique_ptr<Variable> createFloat()
    {
        std::unique_ptr<VariableInput> result = std::make_unique<VariableInput>(nullptr);

        return std::move(result);
    }

    inline static std::unique_ptr<Variable> createEnumeration()
    {
        return nullptr;
    }

    inline static std::unique_ptr<Variable> createString()
    {
        std::unique_ptr<VariableInput> result = std::make_unique<VariableInput>(nullptr);

        return std::move(result);
    }

    inline static std::unique_ptr<Variable> createBool()
    {
        std::unique_ptr<VariableSwitch> result = std::make_unique<VariableSwitch>(nullptr, "Testing");

        return std::move(result);
    }*/
};

}