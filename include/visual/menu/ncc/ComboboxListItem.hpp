/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#pragma once

#include "Button.hpp"
#include "ComboboxList.hpp"


namespace zerokernel
{

class ComboboxListItem : public Button
{
public:
    ComboboxListItem(ComboboxList *parent, std::size_t id);

    void render() override;

    std::size_t id;

protected:
    virtual void onButtonClick() override;
};

}