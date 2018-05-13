/*
  Created by Jenny White on 01.05.18.
  Copyright (c) 2018 nullworks. All rights reserved.
*/

#include <ncc/Button.hpp>
#include <ncc/ComboboxListItem.hpp>

namespace zerokernel
{

ComboboxListItem::ComboboxListItem(ComboboxList *parent, std::size_t id)
        : Button{}, id(id)
{
    setParent(parent);
}

void ComboboxListItem::onButtonClick()
{
    ((ComboboxList *)parent)->box->select(id);
    ((ComboboxList *) parent)->markedForDelete = true;
    ((ComboboxList *)parent)->box->onOverlayClose();
    Button::onButtonClick();
}

void ComboboxListItem::render()
{
    glez::draw::rect_outline(x, y, xSize, ySize, resource::color::foreground, 1);
    utility::drawCenteredString(x + xSize / 2, y + 2, ((ComboboxList *) parent)->box->options.at(id),
                                resource::font::bold, resource::color::text);

    BaseMenuObject::render();
}

}