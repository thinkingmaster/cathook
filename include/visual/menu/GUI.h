/*
 * GUI.h
 *
 *  Created on: Jan 25, 2017
 *      Author: nullifiedcat
 */

#pragma once

class IWidget;
class CatVar;

#include <string>

extern CatVar gui_visible;

class CatMenu
{
public:
    static CatMenu& instance()
    {
        static CatMenu instance{};
        return instance;
    }
    CatMenu(const CatMenu&) = delete;
    void operator=(const CatMenu&) = delete;


};