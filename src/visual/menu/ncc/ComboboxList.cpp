/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include <ncc/ComboboxListItem.hpp>
#include "ncc/ComboboxList.hpp"

namespace zerokernel
{

ComboboxList::ComboboxList(Combobox *box): List{}, box{box}
{
    setParent(box);
    xSize = box->xSize;
    std::size_t id = 0;
    for (auto &s: box->options)
    {
        auto ptr = std::make_unique<ComboboxListItem>(this, id++);
        ptr->xSize = xSize;
        addObject(std::move(ptr));
    }
    interval = -1;
}

}