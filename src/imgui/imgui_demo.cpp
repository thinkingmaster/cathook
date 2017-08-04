
#include "../xorstring.hpp"

// dear imgui, v1.51 WIP
// (demo code)

// Message to the person tempted to delete this file when integrating ImGui into their code base:
// Don't do it! Do NOT remove this file from your project! It is useful reference code that you and other users will want to refer to.
// Everything in this file will be stripped out by the linker if you don't call ImGui::ShowTestWindow().
// During development, you can call ImGui::ShowTestWindow() in your code to learn about various features of ImGui. Have it wired in a debug menu!
// Removing this file from your project is hindering access to documentation for everyone in your team, likely leading you to poorer usage of the library.

// Note that you can #define IMGUI_DISABLE_TEST_WINDOWS in imconfig.h for the same effect.
// If you want to link core ImGui in your public builds but not those test windows, #define IMGUI_DISABLE_TEST_WINDOWS in imconfig.h and those functions will be empty.
// For any other case, if you have ImGui available you probably want this to be available for reference and execution.

// Thank you,
// -Your beloved friend, imgui_demo.cpp (that you won't delete)

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui.h"
#include <ctype.h>          // toupper, isprint
#include <math.h>           // sqrtf, powf, cosf, sinf, floorf, ceilf
#include <stdio.h>          // vsnprintf, sscanf, printf
#include <stdlib.h>         // NULL, malloc, free, atoi
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h>         // intptr_t
#else
#include <stdint.h>         // intptr_t
#endif

#ifdef _MSC_VER
#pragma warning (disable: 4996) // 'This function or variable may be unsafe': strcpy, strdup, sprintf, vsnprintf, sscanf, fopen
#define snprintf _snprintf
#endif
#ifdef __clang__
#pragma clang diagnostic ignored XStr("-Wold-style-cast")             // warning : use of old-style cast                              // yes, they are more terse.
#pragma clang diagnostic ignored XStr("-Wdeprecated-declarations")    // warning : 'xx' is deprecated: The POSIX name for this item.. // for strdup used in demo code (so user can copy & paste the code)
#pragma clang diagnostic ignored XStr("-Wint-to-void-pointer-cast")   // warning : cast to 'void *' from smaller integer type 'int'
#pragma clang diagnostic ignored XStr("-Wformat-security")            // warning : warning: format string is not a string literal
#pragma clang diagnostic ignored XStr("-Wexit-time-destructors")      // warning : declaration requires an exit-time destructor       // exit-time destruction order is undefined. if MemFree() leads to users code that has been disabled before exit it might cause problems. ImGui coding style welcomes static/globals.
#if __has_warning(XStr("-Wreserved-id-macro"))
#pragma clang diagnostic ignored XStr("-Wreserved-id-macro")          // warning : macro name is a reserved identifier                //
#endif
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored XStr("-Wint-to-pointer-cast")          // warning: cast to pointer from integer of different size
#pragma GCC diagnostic ignored XStr("-Wformat-security")              // warning : format string is not a string literal (potentially insecure)
#pragma GCC diagnostic ignored XStr("-Wdouble-promotion")             // warning: implicit conversion from 'float' to 'double' when passing argument to function
#pragma GCC diagnostic ignored XStr("-Wconversion")                   // warning: conversion to 'xxxx' from 'xxxx' may alter its value
#if (__GNUC__ >= 6)
#pragma GCC diagnostic ignored XStr("-Wmisleading-indentation")       // warning: this 'if' clause does not guard this statement      // GCC 6.0+ only. See #883 on github.
#endif
#endif

// Play it nice with Windows users. Notepad in 2015 still doesn't display text data with Unix-style \n.
#ifdef _WIN32
#define IM_NEWLINE XStr("\r\n")
#else
#define IM_NEWLINE XStr("\n")
#endif

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))
#define IM_MAX(_A,_B)       (((_A) >= (_B)) ? (_A) : (_B))

//-----------------------------------------------------------------------------
// DEMO CODE
//-----------------------------------------------------------------------------

#ifndef IMGUI_DISABLE_TEST_WINDOWS

static void ShowExampleAppConsole(bool* p_open);
static void ShowExampleAppLog(bool* p_open);
static void ShowExampleAppLayout(bool* p_open);
static void ShowExampleAppPropertyEditor(bool* p_open);
static void ShowExampleAppLongText(bool* p_open);
static void ShowExampleAppAutoResize(bool* p_open);
static void ShowExampleAppConstrainedResize(bool* p_open);
static void ShowExampleAppFixedOverlay(bool* p_open);
static void ShowExampleAppManipulatingWindowTitle(bool* p_open);
static void ShowExampleAppCustomRendering(bool* p_open);
static void ShowExampleAppMainMenuBar();
static void ShowExampleMenuFile();

static void ShowHelpMarker(const char* desc)
{
    ImGui::TextDisabled(XStr("(?)"));
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(450.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void ImGui::ShowUserGuide()
{
    ImGui::BulletText(XStr("Double-click on title bar to collapse window."));
    ImGui::BulletText(XStr("Click and drag on lower right corner to resize window."));
    ImGui::BulletText(XStr("Click and drag on any empty space to move window."));
    ImGui::BulletText(XStr("Mouse Wheel to scroll."));
    if (ImGui::GetIO().FontAllowUserScaling)
        ImGui::BulletText(XStr("CTRL+Mouse Wheel to zoom window contents."));
    ImGui::BulletText(XStr("TAB/SHIFT+TAB to cycle through keyboard editable fields."));
    ImGui::BulletText(XStr("CTRL+Click on a slider or drag box to input text."));
    ImGui::BulletText(
        XStr("While editing text:\n")
        XStr("- Hold SHIFT or use mouse to select text\n")
        XStr("- CTRL+Left/Right to word jump\n")
        XStr("- CTRL+A or double-click to select all\n")
        XStr("- CTRL+X,CTRL+C,CTRL+V clipboard\n")
        XStr("- CTRL+Z,CTRL+Y undo/redo\n")
        XStr("- ESCAPE to revert\n")
        XStr("- You can apply arithmetic operators +,*,/ on numerical values.\n")
        XStr("  Use +- to subtract.\n"));
}

// Demonstrate most ImGui features (big function!)
void ImGui::ShowTestWindow(bool* p_open)
{
    // Examples apps
    static bool show_app_main_menu_bar = false;
    static bool show_app_console = false;
    static bool show_app_log = false;
    static bool show_app_layout = false;
    static bool show_app_property_editor = false;
    static bool show_app_long_text = false;
    static bool show_app_auto_resize = false;
    static bool show_app_constrained_resize = false;
    static bool show_app_fixed_overlay = false;
    static bool show_app_manipulating_window_title = false;
    static bool show_app_custom_rendering = false;
    static bool show_app_style_editor = false;

    static bool show_app_metrics = false;
    static bool show_app_about = false;

    if (show_app_main_menu_bar) ShowExampleAppMainMenuBar();
    if (show_app_console) ShowExampleAppConsole(&show_app_console);
    if (show_app_log) ShowExampleAppLog(&show_app_log);
    if (show_app_layout) ShowExampleAppLayout(&show_app_layout);
    if (show_app_property_editor) ShowExampleAppPropertyEditor(&show_app_property_editor);
    if (show_app_long_text) ShowExampleAppLongText(&show_app_long_text);
    if (show_app_auto_resize) ShowExampleAppAutoResize(&show_app_auto_resize);
    if (show_app_constrained_resize) ShowExampleAppConstrainedResize(&show_app_constrained_resize);
    if (show_app_fixed_overlay) ShowExampleAppFixedOverlay(&show_app_fixed_overlay);
    if (show_app_manipulating_window_title) ShowExampleAppManipulatingWindowTitle(&show_app_manipulating_window_title);
    if (show_app_custom_rendering) ShowExampleAppCustomRendering(&show_app_custom_rendering);

    if (show_app_metrics) ImGui::ShowMetricsWindow(&show_app_metrics);
    if (show_app_style_editor) { ImGui::Begin(XStr("Style Editor"), &show_app_style_editor); ImGui::ShowStyleEditor(); ImGui::End(); }
    if (show_app_about)
    {
        ImGui::Begin(XStr("About ImGui"), &show_app_about, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text(XStr("dear imgui, %s"), ImGui::GetVersion());
        ImGui::Separator();
        ImGui::Text(XStr("By Omar Cornut and all github contributors."));
        ImGui::Text(XStr("ImGui is licensed under the MIT License, see LICENSE for more information."));
        ImGui::End();
    }

    static bool no_titlebar = false;
    static bool no_border = true;
    static bool no_resize = false;
    static bool no_move = false;
    static bool no_scrollbar = false;
    static bool no_collapse = false;
    static bool no_menu = false;

    // Demonstrate the various window flags. Typically you would just use the default.
    ImGuiWindowFlags window_flags = 0;
    if (no_titlebar)  window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (!no_border)   window_flags |= ImGuiWindowFlags_ShowBorders;
    if (no_resize)    window_flags |= ImGuiWindowFlags_NoResize;
    if (no_move)      window_flags |= ImGuiWindowFlags_NoMove;
    if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (no_collapse)  window_flags |= ImGuiWindowFlags_NoCollapse;
    if (!no_menu)     window_flags |= ImGuiWindowFlags_MenuBar;
    ImGui::SetNextWindowSize(ImVec2(550,680), ImGuiSetCond_FirstUseEver);
    if (!ImGui::Begin(XStr("ImGui Demo"), p_open, window_flags))
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }

    //ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // 2/3 of the space for widget and 1/3 for labels
    ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

    ImGui::Text(XStr("Dear ImGui says hello."));

    // Menu
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu(XStr("Menu")))
        {
            ShowExampleMenuFile();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(XStr("Examples")))
        {
            ImGui::MenuItem(XStr("Main menu bar"), NULL, &show_app_main_menu_bar);
            ImGui::MenuItem(XStr("Console"), NULL, &show_app_console);
            ImGui::MenuItem(XStr("Log"), NULL, &show_app_log);
            ImGui::MenuItem(XStr("Simple layout"), NULL, &show_app_layout);
            ImGui::MenuItem(XStr("Property editor"), NULL, &show_app_property_editor);
            ImGui::MenuItem(XStr("Long text display"), NULL, &show_app_long_text);
            ImGui::MenuItem(XStr("Auto-resizing window"), NULL, &show_app_auto_resize);
            ImGui::MenuItem(XStr("Constrained-resizing window"), NULL, &show_app_constrained_resize);
            ImGui::MenuItem(XStr("Simple overlay"), NULL, &show_app_fixed_overlay);
            ImGui::MenuItem(XStr("Manipulating window title"), NULL, &show_app_manipulating_window_title);
            ImGui::MenuItem(XStr("Custom rendering"), NULL, &show_app_custom_rendering);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(XStr("Help")))
        {
            ImGui::MenuItem(XStr("Metrics"), NULL, &show_app_metrics);
            ImGui::MenuItem(XStr("Style Editor"), NULL, &show_app_style_editor);
            ImGui::MenuItem(XStr("About ImGui"), NULL, &show_app_about);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::Spacing();
    if (ImGui::CollapsingHeader(XStr("Help")))
    {
        ImGui::TextWrapped(XStr("This window is being created by the ShowTestWindow() function. Please refer to the code for programming reference.\n\nUser Guide:"));
        ImGui::ShowUserGuide();
    }

    if (ImGui::CollapsingHeader(XStr("Window options")))
    {
        ImGui::Checkbox(XStr("No titlebar"), &no_titlebar); ImGui::SameLine(150);
        ImGui::Checkbox(XStr("No border"), &no_border); ImGui::SameLine(300);
        ImGui::Checkbox(XStr("No resize"), &no_resize);
        ImGui::Checkbox(XStr("No move"), &no_move); ImGui::SameLine(150);
        ImGui::Checkbox(XStr("No scrollbar"), &no_scrollbar); ImGui::SameLine(300);
        ImGui::Checkbox(XStr("No collapse"), &no_collapse);
        ImGui::Checkbox(XStr("No menu"), &no_menu);

        if (ImGui::TreeNode(XStr("Style")))
        {
            ImGui::ShowStyleEditor();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("Logging")))
        {
            ImGui::TextWrapped(XStr("The logging API redirects all text output so you can easily capture the content of a window or a block. Tree nodes can be automatically expanded. You can also call ImGui::LogText() to output directly to the log without a visual output."));
            ImGui::LogButtons();
            ImGui::TreePop();
        }
    }

    if (ImGui::CollapsingHeader(XStr("Widgets")))
    {
        if (ImGui::TreeNode(XStr("Trees")))
        {
            if (ImGui::TreeNode(XStr("Basic trees")))
            {
                for (int i = 0; i < 5; i++)
                    if (ImGui::TreeNode((void*)(intptr_t)i, XStr("Child %d"), i))
                    {
                        ImGui::Text(XStr("blah blah"));
                        ImGui::SameLine(); 
                        if (ImGui::SmallButton(XStr("print"))) printf(XStr("Child %d pressed"), i);
                        ImGui::TreePop();
                    }
                ImGui::TreePop();
            }

            if (ImGui::TreeNode(XStr("Advanced, with Selectable nodes")))
            {
                ShowHelpMarker(XStr("This is a more standard looking tree with selectable nodes.\nClick to select, CTRL+Click to toggle, click on arrows or double-click to open."));
                static bool align_label_with_current_x_position = false;
                ImGui::Checkbox(XStr("Align label with current X position)"), &align_label_with_current_x_position);
                ImGui::Text(XStr("Hello!"));
                if (align_label_with_current_x_position)
                    ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());

                static int selection_mask = (1 << 2); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
                int node_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
                ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize()*3); // Increase spacing to differentiate leaves from expanded contents.
                for (int i = 0; i < 6; i++)
                {
                    // Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
                    ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ((selection_mask & (1 << i)) ? ImGuiTreeNodeFlags_Selected : 0);
                    if (i < 3)
                    {
                        // Node
                        bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, XStr("Selectable Node %d"), i);
                        if (ImGui::IsItemClicked()) 
                            node_clicked = i;
                        if (node_open)
                        {
                            ImGui::Text(XStr("Blah blah\nBlah Blah"));
                            ImGui::TreePop();
                        }
                    }
                    else
                    {
                        // Leaf: The only reason we have a TreeNode at all is to allow selection of the leaf. Otherwise we can use BulletText() or TreeAdvanceToLabelPos()+Text().
                        ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, XStr("Selectable Leaf %d"), i);
                        if (ImGui::IsItemClicked()) 
                            node_clicked = i;
                    }
                }
                if (node_clicked != -1)
                {
                    // Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
                    if (ImGui::GetIO().KeyCtrl)
                        selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
                    else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
                        selection_mask = (1 << node_clicked);           // Click to single-select
                }
                ImGui::PopStyleVar();
                if (align_label_with_current_x_position)
                    ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("Collapsing Headers")))
        {
            static bool closable_group = true;
            if (ImGui::CollapsingHeader(XStr("Header")))
            {
                ImGui::Checkbox(XStr("Enable extra group"), &closable_group);
                for (int i = 0; i < 5; i++)
                    ImGui::Text(XStr("Some content %d"), i);
            }
            if (ImGui::CollapsingHeader(XStr("Header with a close button"), &closable_group))
            {
                for (int i = 0; i < 5; i++)
                    ImGui::Text(XStr("More content %d"), i);
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("Bullets")))
        {
            ImGui::BulletText(XStr("Bullet point 1"));
            ImGui::BulletText(XStr("Bullet point 2\nOn multiple lines"));
            ImGui::Bullet(); ImGui::Text(XStr("Bullet point 3 (two calls)"));
            ImGui::Bullet(); ImGui::SmallButton(XStr("Button"));
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("Colored Text")))
        {
            // Using shortcut. You can use PushStyleColor()/PopStyleColor() for more flexibility.
            ImGui::TextColored(ImVec4(1.0f,0.0f,1.0f,1.0f), XStr("Pink"));
            ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f), XStr("Yellow"));
            ImGui::TextDisabled(XStr("Disabled"));
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("Word Wrapping")))
        {
            // Using shortcut. You can use PushTextWrapPos()/PopTextWrapPos() for more flexibility.
            ImGui::TextWrapped(XStr("This text should automatically wrap on the edge of the window. The current implementation for text wrapping follows simple rules suitable for English and possibly other languages."));
            ImGui::Spacing();

            static float wrap_width = 200.0f;
            ImGui::SliderFloat(XStr("Wrap width"), &wrap_width, -20, 600, XStr("%.0f"));

            ImGui::Text(XStr("Test paragraph 1:"));
            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x + wrap_width, pos.y), ImVec2(pos.x + wrap_width + 10, pos.y + ImGui::GetTextLineHeight()), IM_COL32(255,0,255,255));
            ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
            ImGui::Text(XStr("The lazy dog is a good dog. This paragraph is made to fit within %.0f pixels. Testing a 1 character word. The quick brown fox jumps over the lazy dog."), wrap_width);
            ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255,255,0,255));
            ImGui::PopTextWrapPos();

            ImGui::Text(XStr("Test paragraph 2:"));
            pos = ImGui::GetCursorScreenPos();
            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(pos.x + wrap_width, pos.y), ImVec2(pos.x + wrap_width + 10, pos.y + ImGui::GetTextLineHeight()), IM_COL32(255,0,255,255));
            ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + wrap_width);
            ImGui::Text(XStr("aaaaaaaa bbbbbbbb, c cccccccc,dddddddd. d eeeeeeee   ffffffff. gggggggg!hhhhhhhh"));
            ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255,255,0,255));
            ImGui::PopTextWrapPos();

            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("UTF-8 Text")))
        {
            // UTF-8 test with Japanese characters
            // (needs a suitable font, try Arial Unicode or M+ fonts http://mplus-fonts.sourceforge.jp/mplus-outline-fonts/index-en.html)
            // Most compiler appears to support UTF-8 in source code (with Visual Studio you need to save your file as 'UTF-8 without signature')
            // However for the sake for maximum portability here we are *not* including raw UTF-8 character in this source file, instead we encode the string with hexadecimal constants.
            // In your own application be reasonable and use UTF-8 in source or retrieve the data from file system!
            // Note that characters values are preserved even if the font cannot be displayed, so you can safely copy & paste garbled characters into another application.
            ImGui::TextWrapped(XStr("CJK text will only appears if the font was loaded with the appropriate CJK character ranges. Call io.Font->LoadFromFileTTF() manually to load extra character ranges."));
            ImGui::Text(XStr("Hiragana: \xe3\x81\x8b\xe3\x81\x8d\xe3\x81\x8f\xe3\x81\x91\xe3\x81\x93 (kakikukeko)"));
            ImGui::Text(XStr("Kanjis: \xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e (nihongo)"));
            static char buf[32] = XStr("\xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e");
            ImGui::InputText(XStr("UTF-8 input"), buf, IM_ARRAYSIZE(buf));
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("Images")))
        {
            ImGui::TextWrapped(XStr("Below we are displaying the font texture (which is the only texture we have access to in this demo). Use the 'ImTextureID' type as storage to pass pointers or identifier to your own texture data. Hover the texture for a zoomed view!"));
            ImVec2 tex_screen_pos = ImGui::GetCursorScreenPos();
            float tex_w = (float)ImGui::GetIO().Fonts->TexWidth;
            float tex_h = (float)ImGui::GetIO().Fonts->TexHeight;
            ImTextureID tex_id = ImGui::GetIO().Fonts->TexID;
            ImGui::Text(XStr("%.0fx%.0f"), tex_w, tex_h);
            ImGui::Image(tex_id, ImVec2(tex_w, tex_h), ImVec2(0,0), ImVec2(1,1), ImColor(255,255,255,255), ImColor(255,255,255,128));
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                float focus_sz = 32.0f;
                float focus_x = ImGui::GetMousePos().x - tex_screen_pos.x - focus_sz * 0.5f; if (focus_x < 0.0f) focus_x = 0.0f; else if (focus_x > tex_w - focus_sz) focus_x = tex_w - focus_sz;
                float focus_y = ImGui::GetMousePos().y - tex_screen_pos.y - focus_sz * 0.5f; if (focus_y < 0.0f) focus_y = 0.0f; else if (focus_y > tex_h - focus_sz) focus_y = tex_h - focus_sz;
                ImGui::Text(XStr("Min: (%.2f, %.2f)"), focus_x, focus_y);
                ImGui::Text(XStr("Max: (%.2f, %.2f)"), focus_x + focus_sz, focus_y + focus_sz);
                ImVec2 uv0 = ImVec2((focus_x) / tex_w, (focus_y) / tex_h);
                ImVec2 uv1 = ImVec2((focus_x + focus_sz) / tex_w, (focus_y + focus_sz) / tex_h);
                ImGui::Image(tex_id, ImVec2(128,128), uv0, uv1, ImColor(255,255,255,255), ImColor(255,255,255,128));
                ImGui::EndTooltip();
            }
            ImGui::TextWrapped(XStr("And now some textured buttons.."));
            static int pressed_count = 0;
            for (int i = 0; i < 8; i++)
            {
                ImGui::PushID(i);
                int frame_padding = -1 + i;     // -1 = uses default padding
                if (ImGui::ImageButton(tex_id, ImVec2(32,32), ImVec2(0,0), ImVec2(32.0f/tex_w,32/tex_h), frame_padding, ImColor(0,0,0,255)))
                    pressed_count += 1;
                ImGui::PopID();
                ImGui::SameLine();
            }
            ImGui::NewLine();
            ImGui::Text(XStr("Pressed %d times."), pressed_count);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("Selectables")))
        {
            if (ImGui::TreeNode(XStr("Basic")))
            {
                static bool selected[4] = { false, true, false, false };
                ImGui::Selectable(XStr("1. I am selectable"), &selected[0]);
                ImGui::Selectable(XStr("2. I am selectable"), &selected[1]);
                ImGui::Text(XStr("3. I am not selectable"));
                ImGui::Selectable(XStr("4. I am selectable"), &selected[2]);
                if (ImGui::Selectable(XStr("5. I am double clickable"), selected[3], ImGuiSelectableFlags_AllowDoubleClick))
                    if (ImGui::IsMouseDoubleClicked(0))
                        selected[3] = !selected[3];
                ImGui::TreePop();
            }
            if (ImGui::TreeNode(XStr("Rendering more text into the same block")))
            {
                static bool selected[3] = { false, false, false };
                ImGui::Selectable(XStr("main.c"), &selected[0]);    ImGui::SameLine(300); ImGui::Text(XStr(" 2,345 bytes"));
                ImGui::Selectable(XStr("Hello.cpp"), &selected[1]); ImGui::SameLine(300); ImGui::Text(XStr("12,345 bytes"));
                ImGui::Selectable(XStr("Hello.h"), &selected[2]);   ImGui::SameLine(300); ImGui::Text(XStr(" 2,345 bytes"));
                ImGui::TreePop();
            }
            if (ImGui::TreeNode(XStr("In columns")))
            {
                ImGui::Columns(3, NULL, false);
                static bool selected[16] = { 0 };
                for (int i = 0; i < 16; i++)
                {
                    char label[32]; sprintf(label, XStr("Item %d"), i);
                    if (ImGui::Selectable(label, &selected[i])) {}
                    ImGui::NextColumn();
                }
                ImGui::Columns(1);
                ImGui::TreePop();
            }
            if (ImGui::TreeNode(XStr("Grid")))
            {
                static bool selected[16] = { true, false, false, false, false, true, false, false, false, false, true, false, false, false, false, true };
                for (int i = 0; i < 16; i++)
                {
                    ImGui::PushID(i);
                    if (ImGui::Selectable(XStr("Sailor"), &selected[i], 0, ImVec2(50,50)))
                    {
                        int x = i % 4, y = i / 4;
                        if (x > 0) selected[i - 1] ^= 1;
                        if (x < 3) selected[i + 1] ^= 1;
                        if (y > 0) selected[i - 4] ^= 1;
                        if (y < 3) selected[i + 4] ^= 1;
                    }
                    if ((i % 4) < 3) ImGui::SameLine();
                    ImGui::PopID();
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("Filtered Text Input")))
        {
            static char buf1[64] = XStr(""); ImGui::InputText(XStr("default"), buf1, 64);
            static char buf2[64] = XStr(""); ImGui::InputText(XStr("decimal"), buf2, 64, ImGuiInputTextFlags_CharsDecimal);
            static char buf3[64] = XStr(""); ImGui::InputText(XStr("hexadecimal"), buf3, 64, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase);
            static char buf4[64] = XStr(""); ImGui::InputText(XStr("uppercase"), buf4, 64, ImGuiInputTextFlags_CharsUppercase);
            static char buf5[64] = XStr(""); ImGui::InputText(XStr("no blank"), buf5, 64, ImGuiInputTextFlags_CharsNoBlank);
            struct TextFilters { static int FilterImGuiLetters(ImGuiTextEditCallbackData* data) { if (data->EventChar < 256 && strchr(XStr("imgui"), (char)data->EventChar)) return 0; return 1; } };
            static char buf6[64] = XStr(""); ImGui::InputText(XStr("\"imgui\" letters"), buf6, 64, ImGuiInputTextFlags_CallbackCharFilter, TextFilters::FilterImGuiLetters);

            ImGui::Text(XStr("Password input"));
            static char bufpass[64] = XStr("password123");
            ImGui::InputText(XStr("password"), bufpass, 64, ImGuiInputTextFlags_Password | ImGuiInputTextFlags_CharsNoBlank);
            ImGui::SameLine(); ShowHelpMarker(XStr("Display all characters as '*'.\nDisable clipboard cut and copy.\nDisable logging.\n"));
            ImGui::InputText(XStr("password (clear)"), bufpass, 64, ImGuiInputTextFlags_CharsNoBlank);

            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("Multi-line Text Input")))
        {
            static bool read_only = false;
            static char text[1024*16] =
                XStr("/*\n")
                XStr(" The Pentium F00F bug, shorthand for F0 0F C7 C8,\n")
                XStr(" the hexadecimal encoding of one offending instruction,\n")
                XStr(" more formally, the invalid operand with locked CMPXCHG8B\n")
                XStr(" instruction bug, is a design flaw in the majority of\n")
                XStr(" Intel Pentium, Pentium MMX, and Pentium OverDrive\n")
                XStr(" processors (all in the P5 microarchitecture).\n")
                XStr("*/\n\n")
                XStr("label:\n")
                XStr("\tlock cmpxchg8b eax\n");

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0,0));
            ImGui::Checkbox(XStr("Read-only"), &read_only);
            ImGui::PopStyleVar();
            ImGui::InputTextMultiline(XStr("##source"), text, IM_ARRAYSIZE(text), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16), ImGuiInputTextFlags_AllowTabInput | (read_only ? ImGuiInputTextFlags_ReadOnly : 0));
            ImGui::TreePop();
        }

        static bool a=false;
        if (ImGui::Button(XStr("Button"))) { printf(XStr("Clicked\n")); a ^= 1; }
        if (a)
        {
            ImGui::SameLine();
            ImGui::Text(XStr("Thanks for clicking me!"));
        }

        static bool check = true;
        ImGui::Checkbox(XStr("checkbox"), &check);

        static int e = 0;
        ImGui::RadioButton(XStr("radio a"), &e, 0); ImGui::SameLine();
        ImGui::RadioButton(XStr("radio b"), &e, 1); ImGui::SameLine();
        ImGui::RadioButton(XStr("radio c"), &e, 2);

        // Color buttons, demonstrate using PushID() to add unique identifier in the ID stack, and changing style.
        for (int i = 0; i < 7; i++)
        {
            if (i > 0) ImGui::SameLine();
            ImGui::PushID(i);
            ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(i/7.0f, 0.6f, 0.6f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(i/7.0f, 0.7f, 0.7f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(i/7.0f, 0.8f, 0.8f));
            ImGui::Button(XStr("Click"));
            ImGui::PopStyleColor(3);
            ImGui::PopID();
        }

        ImGui::Text(XStr("Hover over me"));
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip(XStr("I am a tooltip"));

        ImGui::SameLine();
        ImGui::Text(XStr("- or me"));
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            ImGui::Text(XStr("I am a fancy tooltip"));
            static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
            ImGui::PlotLines(XStr("Curve"), arr, IM_ARRAYSIZE(arr));
            ImGui::EndTooltip();
        }

        // Testing IMGUI_ONCE_UPON_A_FRAME macro
        //for (int i = 0; i < 5; i++)
        //{
        //  IMGUI_ONCE_UPON_A_FRAME
        //  {
        //      ImGui::Text(XStr("This will be displayed only once."));
        //  }
        //}

        ImGui::Separator();

        ImGui::LabelText(XStr("label"), XStr("Value"));

        static int item = 1;
        ImGui::Combo(XStr("combo"), &item, XStr("aaaa\0bbbb\0cccc\0dddd\0eeee\0\0"));   // Combo using values packed in a single constant string (for really quick combo)

        const char* items[] = { XStr("AAAA"), XStr("BBBB"), XStr("CCCC"), XStr("DDDD"), XStr("EEEE"), XStr("FFFF"), XStr("GGGG"), XStr("HHHH"), XStr("IIII"), XStr("JJJJ"), XStr("KKKK") };
        static int item2 = -1;
        ImGui::Combo(XStr("combo scroll"), &item2, items, IM_ARRAYSIZE(items));   // Combo using proper array. You can also pass a callback to retrieve array value, no need to create/copy an array just for that.

        {
            static char str0[128] = XStr("Hello, world!");
            static int i0=123;
            static float f0=0.001f;
            ImGui::InputText(XStr("input text"), str0, IM_ARRAYSIZE(str0));
            ImGui::SameLine(); ShowHelpMarker(XStr("Hold SHIFT or use mouse to select text.\n") XStr("CTRL+Left/Right to word jump.\n") XStr("CTRL+A or double-click to select all.\n") XStr("CTRL+X,CTRL+C,CTRL+V clipboard.\n") XStr("CTRL+Z,CTRL+Y undo/redo.\n") XStr("ESCAPE to revert.\n"));

            ImGui::InputInt(XStr("input int"), &i0);
            ImGui::SameLine(); ShowHelpMarker(XStr("You can apply arithmetic operators +,*,/ on numerical values.\n  e.g. [ 100 ], input \'*2\', result becomes [ 200 ]\nUse +- to subtract.\n"));

            ImGui::InputFloat(XStr("input float"), &f0, 0.01f, 1.0f);

            static float vec4a[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
            ImGui::InputFloat3(XStr("input float3"), vec4a);
        }

        {
            static int i1=50, i2=42;
            ImGui::DragInt(XStr("drag int"), &i1, 1);
            ImGui::SameLine(); ShowHelpMarker(XStr("Click and drag to edit value.\nHold SHIFT/ALT for faster/slower edit.\nDouble-click or CTRL+click to input value."));

            ImGui::DragInt(XStr("drag int 0..100"), &i2, 1, 0, 100, XStr("%.0f%%"));

            static float f1=1.00f, f2=0.0067f;
            ImGui::DragFloat(XStr("drag float"), &f1, 0.005f);
            ImGui::DragFloat(XStr("drag small float"), &f2, 0.0001f, 0.0f, 0.0f, XStr("%.06f ns"));
        }

        {
            static int i1=0;
            ImGui::SliderInt(XStr("slider int"), &i1, -1, 3);
            ImGui::SameLine(); ShowHelpMarker(XStr("CTRL+click to input value."));

            static float f1=0.123f, f2=0.0f;
            ImGui::SliderFloat(XStr("slider float"), &f1, 0.0f, 1.0f, XStr("ratio = %.3f"));
            ImGui::SliderFloat(XStr("slider log float"), &f2, -10.0f, 10.0f, XStr("%.4f"), 3.0f);
            static float angle = 0.0f;
            ImGui::SliderAngle(XStr("slider angle"), &angle);
        }

        static float col1[3] = { 1.0f,0.0f,0.2f };
        static float col2[4] = { 0.4f,0.7f,0.0f,0.5f };
        ImGui::ColorEdit3(XStr("color 1"), col1);
        ImGui::SameLine(); ShowHelpMarker(XStr("Click on the colored square to change edit mode.\nCTRL+click on individual component to input value.\n"));

        ImGui::ColorEdit4(XStr("color 2"), col2);

        const char* listbox_items[] = { XStr("Apple"), XStr("Banana"), XStr("Cherry"), XStr("Kiwi"), XStr("Mango"), XStr("Orange"), XStr("Pineapple"), XStr("Strawberry"), XStr("Watermelon") };
        static int listbox_item_current = 1;
        ImGui::ListBox(XStr("listbox\n(single select)"), &listbox_item_current, listbox_items, IM_ARRAYSIZE(listbox_items), 4);

        //static int listbox_item_current2 = 2;
        //ImGui::PushItemWidth(-1);
        //ImGui::ListBox(XStr("##listbox2"), &listbox_item_current2, listbox_items, IM_ARRAYSIZE(listbox_items), 4);
        //ImGui::PopItemWidth();

        if (ImGui::TreeNode(XStr("Range Widgets")))
        {
            ImGui::Unindent();

            static float begin = 10, end = 90;
            static int begin_i = 100, end_i = 1000;
            ImGui::DragFloatRange2(XStr("range"), &begin, &end, 0.25f, 0.0f, 100.0f, XStr("Min: %.1f %%"), XStr("Max: %.1f %%"));
            ImGui::DragIntRange2(XStr("range int (no bounds)"), &begin_i, &end_i, 5, 0, 0, XStr("Min: %.0f units"), XStr("Max: %.0f units"));

            ImGui::Indent();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("Multi-component Widgets")))
        {
            ImGui::Unindent();

            static float vec4f[4] = { 0.10f, 0.20f, 0.30f, 0.44f };
            static int vec4i[4] = { 1, 5, 100, 255 };

            ImGui::InputFloat2(XStr("input float2"), vec4f);
            ImGui::DragFloat2(XStr("drag float2"), vec4f, 0.01f, 0.0f, 1.0f);
            ImGui::SliderFloat2(XStr("slider float2"), vec4f, 0.0f, 1.0f);
            ImGui::DragInt2(XStr("drag int2"), vec4i, 1, 0, 255);
            ImGui::InputInt2(XStr("input int2"), vec4i);
            ImGui::SliderInt2(XStr("slider int2"), vec4i, 0, 255);
            ImGui::Spacing();

            ImGui::InputFloat3(XStr("input float3"), vec4f);
            ImGui::DragFloat3(XStr("drag float3"), vec4f, 0.01f, 0.0f, 1.0f);
            ImGui::SliderFloat3(XStr("slider float3"), vec4f, 0.0f, 1.0f);
            ImGui::DragInt3(XStr("drag int3"), vec4i, 1, 0, 255);
            ImGui::InputInt3(XStr("input int3"), vec4i);
            ImGui::SliderInt3(XStr("slider int3"), vec4i, 0, 255);
            ImGui::Spacing();

            ImGui::InputFloat4(XStr("input float4"), vec4f);
            ImGui::DragFloat4(XStr("drag float4"), vec4f, 0.01f, 0.0f, 1.0f);
            ImGui::SliderFloat4(XStr("slider float4"), vec4f, 0.0f, 1.0f);
            ImGui::InputInt4(XStr("input int4"), vec4i);
            ImGui::DragInt4(XStr("drag int4"), vec4i, 1, 0, 255);
            ImGui::SliderInt4(XStr("slider int4"), vec4i, 0, 255);

            ImGui::Indent();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("Vertical Sliders")))
        {
            ImGui::Unindent();
            const float spacing = 4;
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(spacing, spacing));

            static int int_value = 0;
            ImGui::VSliderInt(XStr("##int"), ImVec2(18,160), &int_value, 0, 5);
            ImGui::SameLine();

            static float values[7] = { 0.0f, 0.60f, 0.35f, 0.9f, 0.70f, 0.20f, 0.0f };
            ImGui::PushID(XStr("set1"));
            for (int i = 0; i < 7; i++)
            {
                if (i > 0) ImGui::SameLine();
                ImGui::PushID(i);
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColor::HSV(i/7.0f, 0.5f, 0.5f));
                ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImColor::HSV(i/7.0f, 0.6f, 0.5f));
                ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImColor::HSV(i/7.0f, 0.7f, 0.5f));
                ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImColor::HSV(i/7.0f, 0.9f, 0.9f));
                ImGui::VSliderFloat(XStr("##v"), ImVec2(18,160), &values[i], 0.0f, 1.0f, XStr(""));
                if (ImGui::IsItemActive() || ImGui::IsItemHovered())
                    ImGui::SetTooltip(XStr("%.3f"), values[i]);
                ImGui::PopStyleColor(4);
                ImGui::PopID();
            }
            ImGui::PopID();

            ImGui::SameLine();
            ImGui::PushID(XStr("set2"));
            static float values2[4] = { 0.20f, 0.80f, 0.40f, 0.25f };
            const int rows = 3;
            const ImVec2 small_slider_size(18, (160.0f-(rows-1)*spacing)/rows);
            for (int nx = 0; nx < 4; nx++)
            {
                if (nx > 0) ImGui::SameLine();
                ImGui::BeginGroup();
                for (int ny = 0; ny < rows; ny++)
                {
                    ImGui::PushID(nx*rows+ny);
                    ImGui::VSliderFloat(XStr("##v"), small_slider_size, &values2[nx], 0.0f, 1.0f, XStr(""));
                    if (ImGui::IsItemActive() || ImGui::IsItemHovered())
                        ImGui::SetTooltip(XStr("%.3f"), values2[nx]);
                    ImGui::PopID();
                }
                ImGui::EndGroup();
            }
            ImGui::PopID();

            ImGui::SameLine();
            ImGui::PushID(XStr("set3"));
            for (int i = 0; i < 4; i++)
            {
                if (i > 0) ImGui::SameLine();
                ImGui::PushID(i);
                ImGui::PushStyleVar(ImGuiStyleVar_GrabMinSize, 40);
                ImGui::VSliderFloat(XStr("##v"), ImVec2(40,160), &values[i], 0.0f, 1.0f, XStr("%.2f\nsec"));
                ImGui::PopStyleVar();
                ImGui::PopID();
            }
            ImGui::PopID();
            ImGui::PopStyleVar();

            ImGui::Indent();
            ImGui::TreePop();
        }
    }

    if (ImGui::CollapsingHeader(XStr("Graphs widgets")))
    {
        static bool animate = true;
        ImGui::Checkbox(XStr("Animate"), &animate);

        static float arr[] = { 0.6f, 0.1f, 1.0f, 0.5f, 0.92f, 0.1f, 0.2f };
        ImGui::PlotLines(XStr("Frame Times"), arr, IM_ARRAYSIZE(arr));

        // Create a dummy array of contiguous float values to plot
        // Tip: If your float aren't contiguous but part of a structure, you can pass a pointer to your first float and the sizeof() of your structure in the Stride parameter.
        static float values[90] = { 0 };
        static int values_offset = 0;
        if (animate)
        {
            static float refresh_time = ImGui::GetTime(); // Create dummy data at fixed 60 hz rate for the demo
            for (; ImGui::GetTime() > refresh_time + 1.0f/60.0f; refresh_time += 1.0f/60.0f)
            {
                static float phase = 0.0f;
                values[values_offset] = cosf(phase);
                values_offset = (values_offset+1) % IM_ARRAYSIZE(values);
                phase += 0.10f*values_offset;
            }
        }
        ImGui::PlotLines(XStr("Lines"), values, IM_ARRAYSIZE(values), values_offset, XStr("avg 0.0"), -1.0f, 1.0f, ImVec2(0,80));
        ImGui::PlotHistogram(XStr("Histogram"), arr, IM_ARRAYSIZE(arr), 0, NULL, 0.0f, 1.0f, ImVec2(0,80));

        // Use functions to generate output
        // FIXME: This is rather awkward because current plot API only pass in indices. We probably want an API passing floats and user provide sample rate/count.
        struct Funcs
        {
            static float Sin(void*, int i) { return sinf(i * 0.1f); }
            static float Saw(void*, int i) { return (i & 1) ? 1.0f : 0.0f; }
        };
        static int func_type = 0, display_count = 70;
        ImGui::Separator();
        ImGui::PushItemWidth(100); ImGui::Combo(XStr("func"), &func_type, XStr("Sin\0Saw\0")); ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::SliderInt(XStr("Sample count"), &display_count, 1, 400);
        float (*func)(void*, int) = (func_type == 0) ? Funcs::Sin : Funcs::Saw;
        ImGui::PlotLines(XStr("Lines"), func, NULL, display_count, 0, NULL, -1.0f, 1.0f, ImVec2(0,80));
        ImGui::PlotHistogram(XStr("Histogram"), func, NULL, display_count, 0, NULL, -1.0f, 1.0f, ImVec2(0,80));
        ImGui::Separator();

        // Animate a simple progress bar
        static float progress = 0.0f, progress_dir = 1.0f;
        if (animate)
        {
            progress += progress_dir * 0.4f * ImGui::GetIO().DeltaTime;
            if (progress >= +1.1f) { progress = +1.1f; progress_dir *= -1.0f; }
            if (progress <= -0.1f) { progress = -0.1f; progress_dir *= -1.0f; }
        }

        // Typically we would use ImVec2(-1.0f,0.0f) to use all available width, or ImVec2(width,0.0f) for a specified width. ImVec2(0.0f,0.0f) uses ItemWidth.
        ImGui::ProgressBar(progress, ImVec2(0.0f,0.0f));
        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
        ImGui::Text(XStr("Progress Bar"));

        float progress_saturated = (progress < 0.0f) ? 0.0f : (progress > 1.0f) ? 1.0f : progress;
        char buf[32];
        sprintf(buf, XStr("%d/%d"), (int)(progress_saturated*1753), 1753);
        ImGui::ProgressBar(progress, ImVec2(0.f,0.f), buf);
    }

    if (ImGui::CollapsingHeader(XStr("Layout")))
    {
        if (ImGui::TreeNode(XStr("Child regions")))
        {
            ImGui::Text(XStr("Without border"));
            static int line = 50;
            bool goto_line = ImGui::Button(XStr("Goto"));
            ImGui::SameLine();
            ImGui::PushItemWidth(100);
            goto_line |= ImGui::InputInt(XStr("##Line"), &line, 0, 0, ImGuiInputTextFlags_EnterReturnsTrue);
            ImGui::PopItemWidth();
            ImGui::BeginChild(XStr("Sub1"), ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f,300), false, ImGuiWindowFlags_HorizontalScrollbar);
            for (int i = 0; i < 100; i++)
            {
                ImGui::Text(XStr("%04d: scrollable region"), i);
                if (goto_line && line == i)
                    ImGui::SetScrollHere();
            }
            if (goto_line && line >= 100)
                ImGui::SetScrollHere();
            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
            ImGui::BeginChild(XStr("Sub2"), ImVec2(0,300), true);
            ImGui::Text(XStr("With border"));
            ImGui::Columns(2);
            for (int i = 0; i < 100; i++)
            {
                if (i == 50)
                    ImGui::NextColumn();
                char buf[32];
                sprintf(buf, XStr("%08x"), i*5731);
                ImGui::Button(buf, ImVec2(-1.0f, 0.0f));
            }
            ImGui::EndChild();
            ImGui::PopStyleVar();

            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("Widgets Width")))
        {
            static float f = 0.0f;
            ImGui::Text(XStr("PushItemWidth(100)"));
            ImGui::SameLine(); ShowHelpMarker(XStr("Fixed width."));
            ImGui::PushItemWidth(100);
            ImGui::DragFloat(XStr("float##1"), &f);
            ImGui::PopItemWidth();

            ImGui::Text(XStr("PushItemWidth(GetWindowWidth() * 0.5f)"));
            ImGui::SameLine(); ShowHelpMarker(XStr("Half of window width."));
            ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.5f);
            ImGui::DragFloat(XStr("float##2"), &f);
            ImGui::PopItemWidth();

            ImGui::Text(XStr("PushItemWidth(GetContentRegionAvailWidth() * 0.5f)"));
            ImGui::SameLine(); ShowHelpMarker(XStr("Half of available width.\n(~ right-cursor_pos)\n(works within a column set)"));
            ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() * 0.5f);
            ImGui::DragFloat(XStr("float##3"), &f);
            ImGui::PopItemWidth();

            ImGui::Text(XStr("PushItemWidth(-100)"));
            ImGui::SameLine(); ShowHelpMarker(XStr("Align to right edge minus 100"));
            ImGui::PushItemWidth(-100);
            ImGui::DragFloat(XStr("float##4"), &f);
            ImGui::PopItemWidth();

            ImGui::Text(XStr("PushItemWidth(-1)"));
            ImGui::SameLine(); ShowHelpMarker(XStr("Align to right edge"));
            ImGui::PushItemWidth(-1);
            ImGui::DragFloat(XStr("float##5"), &f);
            ImGui::PopItemWidth();

            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("Basic Horizontal Layout")))
        {
            ImGui::TextWrapped(XStr("(Use ImGui::SameLine() to keep adding items to the right of the preceding item)"));

            // Text
            ImGui::Text(XStr("Two items: Hello")); ImGui::SameLine();
            ImGui::TextColored(ImVec4(1,1,0,1), XStr("Sailor"));

            // Adjust spacing
            ImGui::Text(XStr("More spacing: Hello")); ImGui::SameLine(0, 20);
            ImGui::TextColored(ImVec4(1,1,0,1), XStr("Sailor"));

            // Button
            ImGui::AlignFirstTextHeightToWidgets();
            ImGui::Text(XStr("Normal buttons")); ImGui::SameLine();
            ImGui::Button(XStr("Banana")); ImGui::SameLine();
            ImGui::Button(XStr("Apple")); ImGui::SameLine();
            ImGui::Button(XStr("Corniflower"));

            // Button
            ImGui::Text(XStr("Small buttons")); ImGui::SameLine();
            ImGui::SmallButton(XStr("Like this one")); ImGui::SameLine();
            ImGui::Text(XStr("can fit within a text block."));

            // Aligned to arbitrary position. Easy/cheap column.
            ImGui::Text(XStr("Aligned"));
            ImGui::SameLine(150); ImGui::Text(XStr("x=150"));
            ImGui::SameLine(300); ImGui::Text(XStr("x=300"));
            ImGui::Text(XStr("Aligned"));
            ImGui::SameLine(150); ImGui::SmallButton(XStr("x=150"));
            ImGui::SameLine(300); ImGui::SmallButton(XStr("x=300"));

            // Checkbox
            static bool c1=false,c2=false,c3=false,c4=false;
            ImGui::Checkbox(XStr("My"), &c1); ImGui::SameLine();
            ImGui::Checkbox(XStr("Tailor"), &c2); ImGui::SameLine();
            ImGui::Checkbox(XStr("Is"), &c3); ImGui::SameLine();
            ImGui::Checkbox(XStr("Rich"), &c4);

            // Various
            static float f0=1.0f, f1=2.0f, f2=3.0f;
            ImGui::PushItemWidth(80);
            const char* items[] = { XStr("AAAA"), XStr("BBBB"), XStr("CCCC"), XStr("DDDD") };
            static int item = -1;
            ImGui::Combo(XStr("Combo"), &item, items, IM_ARRAYSIZE(items)); ImGui::SameLine();
            ImGui::SliderFloat(XStr("X"), &f0, 0.0f,5.0f); ImGui::SameLine();
            ImGui::SliderFloat(XStr("Y"), &f1, 0.0f,5.0f); ImGui::SameLine();
            ImGui::SliderFloat(XStr("Z"), &f2, 0.0f,5.0f);
            ImGui::PopItemWidth();

            ImGui::PushItemWidth(80);
            ImGui::Text(XStr("Lists:"));
            static int selection[4] = { 0, 1, 2, 3 };
            for (int i = 0; i < 4; i++)
            {
                if (i > 0) ImGui::SameLine();
                ImGui::PushID(i);
                ImGui::ListBox(XStr(""), &selection[i], items, IM_ARRAYSIZE(items));
                ImGui::PopID();
                //if (ImGui::IsItemHovered()) ImGui::SetTooltip(XStr("ListBox %d hovered"), i);
            }
            ImGui::PopItemWidth();

            // Dummy
            ImVec2 sz(30,30);
            ImGui::Button(XStr("A"), sz); ImGui::SameLine();
            ImGui::Dummy(sz); ImGui::SameLine();
            ImGui::Button(XStr("B"), sz);

            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("Groups")))
        {
            ImGui::TextWrapped(XStr("(Using ImGui::BeginGroup()/EndGroup() to layout items. BeginGroup() basically locks the horizontal position. EndGroup() bundles the whole group so that you can use functions such as IsItemHovered() on it.)"));
            ImGui::BeginGroup();
            {
                ImGui::BeginGroup();
                ImGui::Button(XStr("AAA"));
                ImGui::SameLine();
                ImGui::Button(XStr("BBB"));
                ImGui::SameLine();
                ImGui::BeginGroup();
                ImGui::Button(XStr("CCC"));
                ImGui::Button(XStr("DDD"));
                ImGui::EndGroup();
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip(XStr("Group hovered"));
                ImGui::SameLine();
                ImGui::Button(XStr("EEE"));
                ImGui::EndGroup();
            }
            // Capture the group size and create widgets using the same size
            ImVec2 size = ImGui::GetItemRectSize();
            const float values[5] = { 0.5f, 0.20f, 0.80f, 0.60f, 0.25f };
            ImGui::PlotHistogram(XStr("##values"), values, IM_ARRAYSIZE(values), 0, NULL, 0.0f, 1.0f, size);

            ImGui::Button(XStr("ACTION"), ImVec2((size.x - ImGui::GetStyle().ItemSpacing.x)*0.5f,size.y));
            ImGui::SameLine();
            ImGui::Button(XStr("REACTION"), ImVec2((size.x - ImGui::GetStyle().ItemSpacing.x)*0.5f,size.y));
            ImGui::EndGroup();
            ImGui::SameLine();

            ImGui::Button(XStr("LEVERAGE\nBUZZWORD"), size);
            ImGui::SameLine();

            ImGui::ListBoxHeader(XStr("List"), size);
            ImGui::Selectable(XStr("Selected"), true);
            ImGui::Selectable(XStr("Not Selected"), false);
            ImGui::ListBoxFooter();

            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("Text Baseline Alignment")))
        {
            ImGui::TextWrapped(XStr("(This is testing the vertical alignment that occurs on text to keep it at the same baseline as widgets. Lines only composed of text or \"small\" widgets fit in less vertical spaces than lines with normal widgets)"));

            ImGui::Text(XStr("One\nTwo\nThree")); ImGui::SameLine();
            ImGui::Text(XStr("Hello\nWorld")); ImGui::SameLine();
            ImGui::Text(XStr("Banana"));

            ImGui::Text(XStr("Banana")); ImGui::SameLine();
            ImGui::Text(XStr("Hello\nWorld")); ImGui::SameLine();
            ImGui::Text(XStr("One\nTwo\nThree"));

            ImGui::Button(XStr("HOP##1")); ImGui::SameLine();
            ImGui::Text(XStr("Banana")); ImGui::SameLine();
            ImGui::Text(XStr("Hello\nWorld")); ImGui::SameLine();
            ImGui::Text(XStr("Banana"));

            ImGui::Button(XStr("HOP##2")); ImGui::SameLine();
            ImGui::Text(XStr("Hello\nWorld")); ImGui::SameLine();
            ImGui::Text(XStr("Banana"));

            ImGui::Button(XStr("TEST##1")); ImGui::SameLine();
            ImGui::Text(XStr("TEST")); ImGui::SameLine();
            ImGui::SmallButton(XStr("TEST##2"));

            ImGui::AlignFirstTextHeightToWidgets(); // If your line starts with text, call this to align it to upcoming widgets.
            ImGui::Text(XStr("Text aligned to Widget")); ImGui::SameLine();
            ImGui::Button(XStr("Widget##1")); ImGui::SameLine();
            ImGui::Text(XStr("Widget")); ImGui::SameLine();
            ImGui::SmallButton(XStr("Widget##2"));

            // Tree
            const float spacing = ImGui::GetStyle().ItemInnerSpacing.x;
            ImGui::Button(XStr("Button##1"));
            ImGui::SameLine(0.0f, spacing);
            if (ImGui::TreeNode(XStr("Node##1"))) { for (int i = 0; i < 6; i++) ImGui::BulletText(XStr("Item %d.."), i); ImGui::TreePop(); }    // Dummy tree data

            ImGui::AlignFirstTextHeightToWidgets();         // Vertically align text node a bit lower so it'll be vertically centered with upcoming widget. Otherwise you can use SmallButton (smaller fit).
            bool node_open = ImGui::TreeNode(XStr("Node##2"));  // Common mistake to avoid: if we want to SameLine after TreeNode we need to do it before we add child content.
            ImGui::SameLine(0.0f, spacing); ImGui::Button(XStr("Button##2"));
            if (node_open) { for (int i = 0; i < 6; i++) ImGui::BulletText(XStr("Item %d.."), i); ImGui::TreePop(); }   // Dummy tree data

            // Bullet
            ImGui::Button(XStr("Button##3"));
            ImGui::SameLine(0.0f, spacing);
            ImGui::BulletText(XStr("Bullet text"));

            ImGui::AlignFirstTextHeightToWidgets();
            ImGui::BulletText(XStr("Node"));
            ImGui::SameLine(0.0f, spacing); ImGui::Button(XStr("Button##4"));

            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("Scrolling")))
        {
            ImGui::TextWrapped(XStr("(Use SetScrollHere() or SetScrollFromPosY() to scroll to a given position.)"));
            static bool track = true;
            static int track_line = 50, scroll_to_px = 200;
            ImGui::Checkbox(XStr("Track"), &track);
            ImGui::PushItemWidth(100);
            ImGui::SameLine(130); track |= ImGui::DragInt(XStr("##line"), &track_line, 0.25f, 0, 99, XStr("Line %.0f"));
            bool scroll_to = ImGui::Button(XStr("Scroll To"));
            ImGui::SameLine(130); scroll_to |= ImGui::DragInt(XStr("##pos_y"), &scroll_to_px, 1.00f, 0, 9999, XStr("y = %.0f px"));
            ImGui::PopItemWidth();
            if (scroll_to) track = false;

            for (int i = 0; i < 5; i++)
            {
                if (i > 0) ImGui::SameLine();
                ImGui::BeginGroup();
                ImGui::Text(XStr("%s"), i == 0 ? XStr("Top") : i == 1 ? XStr("25%") : i == 2 ? XStr("Center") : i == 3 ? XStr("75%") : XStr("Bottom"));
                ImGui::BeginChild(ImGui::GetID((void*)(intptr_t)i), ImVec2(ImGui::GetWindowWidth() * 0.17f, 200.0f), true);
                if (scroll_to)
                    ImGui::SetScrollFromPosY(ImGui::GetCursorStartPos().y + scroll_to_px, i * 0.25f);
                for (int line = 0; line < 100; line++)
                {
                    if (track && line == track_line)
                    {
                        ImGui::TextColored(ImColor(255,255,0), XStr("Line %d"), line);
                        ImGui::SetScrollHere(i * 0.25f); // 0.0f:top, 0.5f:center, 1.0f:bottom
                    }
                    else
                    {
                        ImGui::Text(XStr("Line %d"), line);
                    }
                }
                ImGui::EndChild();
                ImGui::EndGroup();
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("Horizontal Scrolling")))
        {
            ImGui::Bullet(); ImGui::TextWrapped(XStr("Horizontal scrolling for a window has to be enabled explicitly via the ImGuiWindowFlags_HorizontalScrollbar flag."));
            ImGui::Bullet(); ImGui::TextWrapped(XStr("You may want to explicitly specify content width by calling SetNextWindowContentWidth() before Begin()."));
            static int lines = 7;
            ImGui::SliderInt(XStr("Lines"), &lines, 1, 15);
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 1.0f));
            ImGui::BeginChild(XStr("scrolling"), ImVec2(0, ImGui::GetItemsLineHeightWithSpacing()*7 + 30), true, ImGuiWindowFlags_HorizontalScrollbar);
            for (int line = 0; line < lines; line++)
            {
                // Display random stuff (for the sake of this trivial demo we are using basic Button+SameLine. If you want to create your own time line for a real application you may be better off 
                // manipulating the cursor position yourself, aka using SetCursorPos/SetCursorScreenPos to position the widgets yourself. You may also want to use the lower-level ImDrawList API)
                int num_buttons = 10 + ((line & 1) ? line * 9 : line * 3);
                for (int n = 0; n < num_buttons; n++)
                {
                    if (n > 0) ImGui::SameLine();
                    ImGui::PushID(n + line * 1000);
                    char num_buf[16];
                    const char* label = (!(n%15)) ? XStr("FizzBuzz") : (!(n%3)) ? XStr("Fizz") : (!(n%5)) ? XStr("Buzz") : (sprintf(num_buf, XStr("%d"), n), num_buf);
                    float hue = n*0.05f;
                    ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(hue, 0.6f, 0.6f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(hue, 0.7f, 0.7f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(hue, 0.8f, 0.8f));
                    ImGui::Button(label, ImVec2(40.0f + sinf((float)(line + n)) * 20.0f, 0.0f));
                    ImGui::PopStyleColor(3);
                    ImGui::PopID();
                }
            }
            ImGui::EndChild();
            ImGui::PopStyleVar(2);
            float scroll_x_delta = 0.0f;
            ImGui::SmallButton(XStr("<<")); if (ImGui::IsItemActive()) scroll_x_delta = -ImGui::GetIO().DeltaTime * 1000.0f;
            ImGui::SameLine(); ImGui::Text(XStr("Scroll from code")); ImGui::SameLine();
            ImGui::SmallButton(XStr(">>")); if (ImGui::IsItemActive()) scroll_x_delta = +ImGui::GetIO().DeltaTime * 1000.0f;
            if (scroll_x_delta != 0.0f)
            {
                ImGui::BeginChild(XStr("scrolling")); // Demonstrate a trick: you can use Begin to set yourself in the context of another window (here we are already out of your child window)
                ImGui::SetScrollX(ImGui::GetScrollX() + scroll_x_delta);
                ImGui::End();
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("Clipping")))
        {
            static ImVec2 size(100, 100), offset(50, 20);
            ImGui::TextWrapped(XStr("On a per-widget basis we are occasionally clipping text CPU-side if it won't fit in its frame. Otherwise we are doing coarser clipping + passing a scissor rectangle to the renderer. The system is designed to try minimizing both execution and CPU/GPU rendering cost."));
            ImGui::DragFloat2(XStr("size"), (float*)&size, 0.5f, 0.0f, 200.0f, XStr("%.0f"));
            ImGui::TextWrapped(XStr("(Click and drag)"));
            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImVec4 clip_rect(pos.x, pos.y, pos.x+size.x, pos.y+size.y);
            ImGui::InvisibleButton(XStr("##dummy"), size);
            if (ImGui::IsItemActive() && ImGui::IsMouseDragging()) { offset.x += ImGui::GetIO().MouseDelta.x; offset.y += ImGui::GetIO().MouseDelta.y; }
            ImGui::GetWindowDrawList()->AddRectFilled(pos, ImVec2(pos.x+size.x,pos.y+size.y), ImColor(90,90,120,255));
            ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize()*2.0f, ImVec2(pos.x+offset.x,pos.y+offset.y), ImColor(255,255,255,255), XStr("Line 1 hello\nLine 2 clip me!"), NULL, 0.0f, &clip_rect);
            ImGui::TreePop();
        }
    }

    if (ImGui::CollapsingHeader(XStr("Popups & Modal windows")))
    {
        if (ImGui::TreeNode(XStr("Popups")))
        {
            ImGui::TextWrapped(XStr("When a popup is active, it inhibits interacting with windows that are behind the popup. Clicking outside the popup closes it."));

            static int selected_fish = -1;
            const char* names[] = { XStr("Bream"), XStr("Haddock"), XStr("Mackerel"), XStr("Pollock"), XStr("Tilefish") };
            static bool toggles[] = { true, false, false, false, false };

            // Simple selection popup
            // (If you want to show the current selection inside the Button itself, you may want to build a string using the XStr("###") operator to preserve a constant ID with a variable label)
            if (ImGui::Button(XStr("Select..")))
                ImGui::OpenPopup(XStr("select"));
            ImGui::SameLine();
            ImGui::Text(selected_fish == -1 ? XStr("<None>") : names[selected_fish]);
            if (ImGui::BeginPopup(XStr("select")))
            {
                ImGui::Text(XStr("Aquarium"));
                ImGui::Separator();
                for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                    if (ImGui::Selectable(names[i]))
                        selected_fish = i;
                ImGui::EndPopup();
            }

            // Showing a menu with toggles
            if (ImGui::Button(XStr("Toggle..")))
                ImGui::OpenPopup(XStr("toggle"));
            if (ImGui::BeginPopup(XStr("toggle")))
            {
                for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                    ImGui::MenuItem(names[i], XStr(""), &toggles[i]);
                if (ImGui::BeginMenu(XStr("Sub-menu")))
                {
                    ImGui::MenuItem(XStr("Click me"));
                    ImGui::EndMenu();
                }

                ImGui::Separator();
                ImGui::Text(XStr("Tooltip here"));
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip(XStr("I am a tooltip over a popup"));

                if (ImGui::Button(XStr("Stacked Popup")))
                    ImGui::OpenPopup(XStr("another popup"));
                if (ImGui::BeginPopup(XStr("another popup")))
                {
                    for (int i = 0; i < IM_ARRAYSIZE(names); i++)
                        ImGui::MenuItem(names[i], XStr(""), &toggles[i]);
                    if (ImGui::BeginMenu(XStr("Sub-menu")))
                    {
                        ImGui::MenuItem(XStr("Click me"));
                        ImGui::EndMenu();
                    }
                    ImGui::EndPopup();
                }
                ImGui::EndPopup();
            }

            if (ImGui::Button(XStr("Popup Menu..")))
                ImGui::OpenPopup(XStr("FilePopup"));
            if (ImGui::BeginPopup(XStr("FilePopup")))
            {
                ShowExampleMenuFile();
                ImGui::EndPopup();
            }

            ImGui::Spacing();
            ImGui::TextWrapped(XStr("Below we are testing adding menu items to a regular window. It's rather unusual but should work!"));
            ImGui::Separator();
            // NB: As a quirk in this very specific example, we want to differentiate the parent of this menu from the parent of the various popup menus above.
            // To do so we are encloding the items in a PushID()/PopID() block to make them two different menusets. If we don't, opening any popup above and hovering our menu here
            // would open it. This is because once a menu is active, we allow to switch to a sibling menu by just hovering on it, which is the desired behavior for regular menus.
            ImGui::PushID(XStr("foo"));
            ImGui::MenuItem(XStr("Menu item"), XStr("CTRL+M"));
            if (ImGui::BeginMenu(XStr("Menu inside a regular window")))
            {
                ShowExampleMenuFile();
                ImGui::EndMenu();
            }
            ImGui::PopID();
            ImGui::Separator();

            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("Context menus")))
        {
            static float value = 0.5f;
            ImGui::Text(XStr("Value = %.3f (<-- right-click here)"), value);
            if (ImGui::BeginPopupContextItem(XStr("item context menu")))
            {
                if (ImGui::Selectable(XStr("Set to zero"))) value = 0.0f;
                if (ImGui::Selectable(XStr("Set to PI"))) value = 3.1415f;
                ImGui::EndPopup();
            }

            static ImVec4 color = ImColor(0.8f, 0.5f, 1.0f, 1.0f);
            ImGui::ColorButton(color);
            if (ImGui::BeginPopupContextItem(XStr("color context menu")))
            {
                ImGui::Text(XStr("Edit color"));
                ImGui::ColorEdit3(XStr("##edit"), (float*)&color);
                if (ImGui::Button(XStr("Close")))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }
            ImGui::SameLine(); ImGui::Text(XStr("(<-- right-click here)"));

            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("Modals")))
        {
            ImGui::TextWrapped(XStr("Modal windows are like popups but the user cannot close them by clicking outside the window."));

            if (ImGui::Button(XStr("Delete..")))
                ImGui::OpenPopup(XStr("Delete?"));
            if (ImGui::BeginPopupModal(XStr("Delete?"), NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text(XStr("All those beautiful files will be deleted.\nThis operation cannot be undone!\n\n"));
                ImGui::Separator();

                //static int dummy_i = 0;
                //ImGui::Combo(XStr("Combo"), &dummy_i, XStr("Delete\0Delete harder\0"));

                static bool dont_ask_me_next_time = false;
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0,0));
                ImGui::Checkbox(XStr("Don't ask me next time"), &dont_ask_me_next_time);
                ImGui::PopStyleVar();

                if (ImGui::Button(XStr("OK"), ImVec2(120,0))) { ImGui::CloseCurrentPopup(); }
                ImGui::SameLine();
                if (ImGui::Button(XStr("Cancel"), ImVec2(120,0))) { ImGui::CloseCurrentPopup(); }
                ImGui::EndPopup();
            }

            if (ImGui::Button(XStr("Stacked modals..")))
                ImGui::OpenPopup(XStr("Stacked 1"));
            if (ImGui::BeginPopupModal(XStr("Stacked 1")))
            {
                ImGui::Text(XStr("Hello from Stacked The First"));

                if (ImGui::Button(XStr("Another one..")))
                    ImGui::OpenPopup(XStr("Stacked 2"));
                if (ImGui::BeginPopupModal(XStr("Stacked 2")))
                {
                    ImGui::Text(XStr("Hello from Stacked The Second"));
                    if (ImGui::Button(XStr("Close")))
                        ImGui::CloseCurrentPopup();
                    ImGui::EndPopup();
                }

                if (ImGui::Button(XStr("Close")))
                    ImGui::CloseCurrentPopup();
                ImGui::EndPopup();
            }

            ImGui::TreePop();
        }
    }

    if (ImGui::CollapsingHeader(XStr("Columns")))
    {
        // Basic columns
        if (ImGui::TreeNode(XStr("Basic")))
        {
            ImGui::Text(XStr("Without border:"));
            ImGui::Columns(3, XStr("mycolumns3"), false);  // 3-ways, no border
            ImGui::Separator();
            for (int n = 0; n < 14; n++)
            {
                char label[32];
                sprintf(label, XStr("Item %d"), n);
                if (ImGui::Selectable(label)) {}
                //if (ImGui::Button(label, ImVec2(-1,0))) {}
                ImGui::NextColumn();
            }
            ImGui::Columns(1);
            ImGui::Separator();

            ImGui::Text(XStr("With border:"));
            ImGui::Columns(4, XStr("mycolumns")); // 4-ways, with border
            ImGui::Separator();
            ImGui::Text(XStr("ID")); ImGui::NextColumn();
            ImGui::Text(XStr("Name")); ImGui::NextColumn();
            ImGui::Text(XStr("Path")); ImGui::NextColumn();
            ImGui::Text(XStr("Flags")); ImGui::NextColumn();
            ImGui::Separator();
            const char* names[3] = { XStr("One"), XStr("Two"), XStr("Three") };
            const char* paths[3] = { XStr("/path/one"), XStr("/path/two"), XStr("/path/three") };
            static int selected = -1;
            for (int i = 0; i < 3; i++)
            {
                char label[32];
                sprintf(label, XStr("%04d"), i);
                if (ImGui::Selectable(label, selected == i, ImGuiSelectableFlags_SpanAllColumns))
                    selected = i;
                ImGui::NextColumn();
                ImGui::Text(names[i]); ImGui::NextColumn();
                ImGui::Text(paths[i]); ImGui::NextColumn();
                ImGui::Text(XStr("....")); ImGui::NextColumn();
            }
            ImGui::Columns(1);
            ImGui::Separator();
            ImGui::TreePop();
        }

        // Scrolling columns
        /*
        if (ImGui::TreeNode(XStr("Scrolling")))
        {
            ImGui::BeginChild(XStr("##header"), ImVec2(0, ImGui::GetTextLineHeightWithSpacing()+ImGui::GetStyle().ItemSpacing.y));
            ImGui::Columns(3);
            ImGui::Text(XStr("ID")); ImGui::NextColumn();
            ImGui::Text(XStr("Name")); ImGui::NextColumn();
            ImGui::Text(XStr("Path")); ImGui::NextColumn();
            ImGui::Columns(1);
            ImGui::Separator();
            ImGui::EndChild();
            ImGui::BeginChild(XStr("##scrollingregion"), ImVec2(0, 60));
            ImGui::Columns(3);
            for (int i = 0; i < 10; i++)
            {
                ImGui::Text(XStr("%04d"), i); ImGui::NextColumn();
                ImGui::Text(XStr("Foobar")); ImGui::NextColumn();
                ImGui::Text(XStr("/path/foobar/%04d/"), i); ImGui::NextColumn();
            }
            ImGui::Columns(1);
            ImGui::EndChild();
            ImGui::TreePop();
        }
        */

        // Create multiple items in a same cell before switching to next column
        if (ImGui::TreeNode(XStr("Mixed items")))
        {
            ImGui::Columns(3, XStr("mixed"));
            ImGui::Separator();

            ImGui::Text(XStr("Hello"));
            ImGui::Button(XStr("Banana"));
            ImGui::NextColumn();

            ImGui::Text(XStr("ImGui"));
            ImGui::Button(XStr("Apple"));
            static float foo = 1.0f;
            ImGui::InputFloat(XStr("red"), &foo, 0.05f, 0, 3);
            ImGui::Text(XStr("An extra line here."));
            ImGui::NextColumn();

            ImGui::Text(XStr("Sailor"));
            ImGui::Button(XStr("Corniflower"));
            static float bar = 1.0f;
            ImGui::InputFloat(XStr("blue"), &bar, 0.05f, 0, 3);
            ImGui::NextColumn();

            if (ImGui::CollapsingHeader(XStr("Category A"))) ImGui::Text(XStr("Blah blah blah")); ImGui::NextColumn();
            if (ImGui::CollapsingHeader(XStr("Category B"))) ImGui::Text(XStr("Blah blah blah")); ImGui::NextColumn();
            if (ImGui::CollapsingHeader(XStr("Category C"))) ImGui::Text(XStr("Blah blah blah")); ImGui::NextColumn();
            ImGui::Columns(1);
            ImGui::Separator();
            ImGui::TreePop();
        }

        // Word wrapping
        if (ImGui::TreeNode(XStr("Word-wrapping")))
        {
            ImGui::Columns(2, XStr("word-wrapping"));
            ImGui::Separator();
            ImGui::TextWrapped(XStr("The quick brown fox jumps over the lazy dog."));
            ImGui::TextWrapped(XStr("Hello Left"));
            ImGui::NextColumn();
            ImGui::TextWrapped(XStr("The quick brown fox jumps over the lazy dog."));
            ImGui::TextWrapped(XStr("Hello Right"));
            ImGui::Columns(1);
            ImGui::Separator();
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("Borders")))
        {
            static bool h_borders = true;
            static bool v_borders = true;
            ImGui::Checkbox(XStr("horizontal"), &h_borders);
            ImGui::SameLine();
            ImGui::Checkbox(XStr("vertical"), &v_borders);
            ImGui::Columns(4, NULL, v_borders);
            if (h_borders) ImGui::Separator();
            for (int i = 0; i < 8; i++)
            {
                ImGui::Text(XStr("%c%c%c"), 'a'+i, 'a'+i, 'a'+i);
                ImGui::NextColumn();
            }
            ImGui::Columns(1);
            if (h_borders) ImGui::Separator();
            ImGui::TreePop();
        }

        bool node_open = ImGui::TreeNode(XStr("Tree within single cell"));
        ImGui::SameLine(); ShowHelpMarker(XStr("NB: Tree node must be poped before ending the cell.\nThere's no storage of state per-cell."));
        if (node_open)
        {
            ImGui::Columns(2, XStr("tree items"));
            ImGui::Separator();
            if (ImGui::TreeNode(XStr("Hello"))) { ImGui::BulletText(XStr("Sailor")); ImGui::TreePop(); } ImGui::NextColumn();
            if (ImGui::TreeNode(XStr("Bonjour"))) { ImGui::BulletText(XStr("Marin")); ImGui::TreePop(); } ImGui::NextColumn();
            ImGui::Columns(1);
            ImGui::Separator();
            ImGui::TreePop();
        }
    }

    if (ImGui::CollapsingHeader(XStr("Filtering")))
    {
        static ImGuiTextFilter filter;
        ImGui::Text(XStr("Filter usage:\n")
                    XStr("  \"\"         display all lines\n")
                    XStr("  \"xxx\"      display lines containing \"xxx\"\n")
                    XStr("  \"xxx,yyy\"  display lines containing \"xxx\" or \"yyy\"\n")
                    XStr("  \"-xxx\"     hide lines containing \"xxx\""));
        filter.Draw();
        const char* lines[] = { XStr("aaa1.c"), XStr("bbb1.c"), XStr("ccc1.c"), XStr("aaa2.cpp"), XStr("bbb2.cpp"), XStr("ccc2.cpp"), XStr("abc.h"), XStr("hello, world") };
        for (int i = 0; i < IM_ARRAYSIZE(lines); i++)
            if (filter.PassFilter(lines[i]))
                ImGui::BulletText(XStr("%s"), lines[i]);
    }

    if (ImGui::CollapsingHeader(XStr("Keyboard, Mouse & Focus")))
    {
        if (ImGui::TreeNode(XStr("Tabbing")))
        {
            ImGui::Text(XStr("Use TAB/SHIFT+TAB to cycle through keyboard editable fields."));
            static char buf[32] = XStr("dummy");
            ImGui::InputText(XStr("1"), buf, IM_ARRAYSIZE(buf));
            ImGui::InputText(XStr("2"), buf, IM_ARRAYSIZE(buf));
            ImGui::InputText(XStr("3"), buf, IM_ARRAYSIZE(buf));
            ImGui::PushAllowKeyboardFocus(false);
            ImGui::InputText(XStr("4 (tab skip)"), buf, IM_ARRAYSIZE(buf));
            //ImGui::SameLine(); ShowHelperMarker(XStr("Use ImGui::PushAllowKeyboardFocus(bool)\nto disable tabbing through certain widgets."));
            ImGui::PopAllowKeyboardFocus();
            ImGui::InputText(XStr("5"), buf, IM_ARRAYSIZE(buf));
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("Focus from code")))
        {
            bool focus_1 = ImGui::Button(XStr("Focus on 1")); ImGui::SameLine();
            bool focus_2 = ImGui::Button(XStr("Focus on 2")); ImGui::SameLine();
            bool focus_3 = ImGui::Button(XStr("Focus on 3"));
            int has_focus = 0;
            static char buf[128] = XStr("click on a button to set focus");

            if (focus_1) ImGui::SetKeyboardFocusHere();
            ImGui::InputText(XStr("1"), buf, IM_ARRAYSIZE(buf));
            if (ImGui::IsItemActive()) has_focus = 1;

            if (focus_2) ImGui::SetKeyboardFocusHere();
            ImGui::InputText(XStr("2"), buf, IM_ARRAYSIZE(buf));
            if (ImGui::IsItemActive()) has_focus = 2;

            ImGui::PushAllowKeyboardFocus(false);
            if (focus_3) ImGui::SetKeyboardFocusHere();
            ImGui::InputText(XStr("3 (tab skip)"), buf, IM_ARRAYSIZE(buf));
            if (ImGui::IsItemActive()) has_focus = 3;
            ImGui::PopAllowKeyboardFocus();
            if (has_focus)
                ImGui::Text(XStr("Item with focus: %d"), has_focus);
            else
                ImGui::Text(XStr("Item with focus: <none>"));
            ImGui::TextWrapped(XStr("Cursor & selection are preserved when refocusing last used item in code."));
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("Dragging")))
        {
            ImGui::TextWrapped(XStr("You can use ImGui::GetMouseDragDelta(0) to query for the dragged amount on any widget."));
            ImGui::Button(XStr("Drag Me"));
            if (ImGui::IsItemActive())
            {
                // Draw a line between the button and the mouse cursor
                ImDrawList* draw_list = ImGui::GetWindowDrawList();
                draw_list->PushClipRectFullScreen();
                draw_list->AddLine(ImGui::CalcItemRectClosestPoint(ImGui::GetIO().MousePos, true, -2.0f), ImGui::GetIO().MousePos, ImColor(ImGui::GetStyle().Colors[ImGuiCol_Button]), 4.0f);
                draw_list->PopClipRect();
                ImVec2 value_raw = ImGui::GetMouseDragDelta(0, 0.0f);
                ImVec2 value_with_lock_threshold = ImGui::GetMouseDragDelta(0);
                ImVec2 mouse_delta = ImGui::GetIO().MouseDelta;
                ImGui::SameLine(); ImGui::Text(XStr("Raw (%.1f, %.1f), WithLockThresold (%.1f, %.1f), MouseDelta (%.1f, %.1f)"), value_raw.x, value_raw.y, value_with_lock_threshold.x, value_with_lock_threshold.y, mouse_delta.x, mouse_delta.y);
            }
            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("Keyboard & Mouse State")))
        {
            ImGuiIO& io = ImGui::GetIO();

            ImGui::Text(XStr("MousePos: (%g, %g)"), io.MousePos.x, io.MousePos.y);
            ImGui::Text(XStr("Mouse down:"));     for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (io.MouseDownDuration[i] >= 0.0f)   { ImGui::SameLine(); ImGui::Text(XStr("b%d (%.02f secs)"), i, io.MouseDownDuration[i]); }
            ImGui::Text(XStr("Mouse clicked:"));  for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseClicked(i))          { ImGui::SameLine(); ImGui::Text(XStr("b%d"), i); }
            ImGui::Text(XStr("Mouse dbl-clicked:")); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDoubleClicked(i)) { ImGui::SameLine(); ImGui::Text(XStr("b%d"), i); }
            ImGui::Text(XStr("Mouse released:")); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseReleased(i))         { ImGui::SameLine(); ImGui::Text(XStr("b%d"), i); }
            ImGui::Text(XStr("MouseWheel: %.1f"), io.MouseWheel);

            ImGui::Text(XStr("Keys down:"));      for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (io.KeysDownDuration[i] >= 0.0f)     { ImGui::SameLine(); ImGui::Text(XStr("%d (%.02f secs)"), i, io.KeysDownDuration[i]); }
            ImGui::Text(XStr("Keys pressed:"));   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyPressed(i))             { ImGui::SameLine(); ImGui::Text(XStr("%d"), i); }
            ImGui::Text(XStr("Keys release:"));   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyReleased(i))            { ImGui::SameLine(); ImGui::Text(XStr("%d"), i); }
            ImGui::Text(XStr("KeyMods: %s%s%s%s"), io.KeyCtrl ? XStr("CTRL ") : XStr(""), io.KeyShift ? XStr("SHIFT ") : XStr(""), io.KeyAlt ? XStr("ALT ") : XStr(""), io.KeySuper ? XStr("SUPER ") : XStr(""));

            ImGui::Text(XStr("WantCaptureMouse: %s"), io.WantCaptureMouse ? XStr("true") : XStr("false"));
            ImGui::Text(XStr("WantCaptureKeyboard: %s"), io.WantCaptureKeyboard ? XStr("true") : XStr("false"));
            ImGui::Text(XStr("WantTextInput: %s"), io.WantTextInput ? XStr("true") : XStr("false"));

            ImGui::Button(XStr("Hovering me sets the\nkeyboard capture flag"));
            if (ImGui::IsItemHovered())
                ImGui::CaptureKeyboardFromApp(true);
            ImGui::SameLine();
            ImGui::Button(XStr("Holding me clears the\nthe keyboard capture flag"));
            if (ImGui::IsItemActive())
                ImGui::CaptureKeyboardFromApp(false);

            ImGui::TreePop();
        }

        if (ImGui::TreeNode(XStr("Mouse cursors")))
        {
            ImGui::TextWrapped(XStr("Your application can render a different mouse cursor based on what ImGui::GetMouseCursor() returns. You can also set io.MouseDrawCursor to ask ImGui to render the cursor for you in software."));
            ImGui::Checkbox(XStr("io.MouseDrawCursor"), &ImGui::GetIO().MouseDrawCursor);
            ImGui::Text(XStr("Hover to see mouse cursors:"));
            for (int i = 0; i < ImGuiMouseCursor_Count_; i++)
            {
                char label[32];
                sprintf(label, XStr("Mouse cursor %d"), i);
                ImGui::Bullet(); ImGui::Selectable(label, false);
                if (ImGui::IsItemHovered())
                    ImGui::SetMouseCursor(i);
            }
            ImGui::TreePop();
        }
    }

    ImGui::End();
}

void ImGui::ShowStyleEditor(ImGuiStyle* ref)
{
    ImGuiStyle& style = ImGui::GetStyle();

    // You can pass in a reference ImGuiStyle structure to compare to, revert to and save to (else it compares to the default style)
    const ImGuiStyle default_style; // Default style
    if (ImGui::Button(XStr("Revert Style")))
        style = ref ? *ref : default_style;

    if (ref)
    {
        ImGui::SameLine();
        if (ImGui::Button(XStr("Save Style")))
            *ref = style;
    }

    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.55f);

    if (ImGui::TreeNode(XStr("Rendering")))
    {
        ImGui::Checkbox(XStr("Anti-aliased lines"), &style.AntiAliasedLines);
        ImGui::Checkbox(XStr("Anti-aliased shapes"), &style.AntiAliasedShapes);
        ImGui::PushItemWidth(100);
        ImGui::DragFloat(XStr("Curve Tessellation Tolerance"), &style.CurveTessellationTol, 0.02f, 0.10f, FLT_MAX, NULL, 2.0f);
        if (style.CurveTessellationTol < 0.0f) style.CurveTessellationTol = 0.10f;
        ImGui::DragFloat(XStr("Global Alpha"), &style.Alpha, 0.005f, 0.20f, 1.0f, XStr("%.2f")); // Not exposing zero here so user doesn't XStr("lose") the UI (zero alpha clips all widgets). But application code could have a toggle to switch between zero and non-zero.
        ImGui::PopItemWidth();
        ImGui::TreePop();
    }

    if (ImGui::TreeNode(XStr("Settings")))
    {
        ImGui::SliderFloat2(XStr("WindowPadding"), (float*)&style.WindowPadding, 0.0f, 20.0f, XStr("%.0f"));
        ImGui::SliderFloat(XStr("WindowRounding"), &style.WindowRounding, 0.0f, 16.0f, XStr("%.0f"));
        ImGui::SliderFloat(XStr("ChildWindowRounding"), &style.ChildWindowRounding, 0.0f, 16.0f, XStr("%.0f"));
        ImGui::SliderFloat2(XStr("FramePadding"), (float*)&style.FramePadding, 0.0f, 20.0f, XStr("%.0f"));
        ImGui::SliderFloat(XStr("FrameRounding"), &style.FrameRounding, 0.0f, 16.0f, XStr("%.0f"));
        ImGui::SliderFloat2(XStr("ItemSpacing"), (float*)&style.ItemSpacing, 0.0f, 20.0f, XStr("%.0f"));
        ImGui::SliderFloat2(XStr("ItemInnerSpacing"), (float*)&style.ItemInnerSpacing, 0.0f, 20.0f, XStr("%.0f"));
        ImGui::SliderFloat2(XStr("TouchExtraPadding"), (float*)&style.TouchExtraPadding, 0.0f, 10.0f, XStr("%.0f"));
        ImGui::SliderFloat(XStr("IndentSpacing"), &style.IndentSpacing, 0.0f, 30.0f, XStr("%.0f"));
        ImGui::SliderFloat(XStr("ScrollbarSize"), &style.ScrollbarSize, 1.0f, 20.0f, XStr("%.0f"));
        ImGui::SliderFloat(XStr("ScrollbarRounding"), &style.ScrollbarRounding, 0.0f, 16.0f, XStr("%.0f"));
        ImGui::SliderFloat(XStr("GrabMinSize"), &style.GrabMinSize, 1.0f, 20.0f, XStr("%.0f"));
        ImGui::SliderFloat(XStr("GrabRounding"), &style.GrabRounding, 0.0f, 16.0f, XStr("%.0f"));
        ImGui::Text(XStr("Alignment"));
        ImGui::SliderFloat2(XStr("WindowTitleAlign"), (float*)&style.WindowTitleAlign, 0.0f, 1.0f, XStr("%.2f"));
        ImGui::SliderFloat2(XStr("ButtonTextAlign"), (float*)&style.ButtonTextAlign, 0.0f, 1.0f, XStr("%.2f")); ImGui::SameLine(); ShowHelpMarker(XStr("Alignment applies when a button is larger than its text content."));
        ImGui::TreePop();
    }

    if (ImGui::TreeNode(XStr("Colors")))
    {
        static int output_dest = 0;
        static bool output_only_modified = false;
        if (ImGui::Button(XStr("Copy Colors")))
        {
            if (output_dest == 0)
                ImGui::LogToClipboard();
            else
                ImGui::LogToTTY();
            ImGui::LogText(XStr("ImGuiStyle& style = ImGui::GetStyle();") IM_NEWLINE);
            for (int i = 0; i < ImGuiCol_COUNT; i++)
            {
                const ImVec4& col = style.Colors[i];
                const char* name = ImGui::GetStyleColName(i);
                if (!output_only_modified || memcmp(&col, (ref ? &ref->Colors[i] : &default_style.Colors[i]), sizeof(ImVec4)) != 0)
                    ImGui::LogText(XStr("style.Colors[ImGuiCol_%s]%*s= ImVec4(%.2ff, %.2ff, %.2ff, %.2ff);") IM_NEWLINE, name, 22 - (int)strlen(name), XStr(""), col.x, col.y, col.z, col.w);
            }
            ImGui::LogFinish();
        }
        ImGui::SameLine(); ImGui::PushItemWidth(120); ImGui::Combo(XStr("##output_type"), &output_dest, XStr("To Clipboard\0To TTY\0")); ImGui::PopItemWidth();
        ImGui::SameLine(); ImGui::Checkbox(XStr("Only Modified Fields"), &output_only_modified);

        static ImGuiColorEditMode edit_mode = ImGuiColorEditMode_RGB;
        ImGui::RadioButton(XStr("RGB"), &edit_mode, ImGuiColorEditMode_RGB);
        ImGui::SameLine();
        ImGui::RadioButton(XStr("HSV"), &edit_mode, ImGuiColorEditMode_HSV);
        ImGui::SameLine();
        ImGui::RadioButton(XStr("HEX"), &edit_mode, ImGuiColorEditMode_HEX);
        //ImGui::Text(XStr("Tip: Click on colored square to change edit mode."));

        static ImGuiTextFilter filter;
        filter.Draw(XStr("Filter colors"), 200);

        ImGui::BeginChild(XStr("#colors"), ImVec2(0, 300), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        ImGui::PushItemWidth(-160);
        ImGui::ColorEditMode(edit_mode);
        for (int i = 0; i < ImGuiCol_COUNT; i++)
        {
            const char* name = ImGui::GetStyleColName(i);
            if (!filter.PassFilter(name))
                continue;
            ImGui::PushID(i);
            ImGui::ColorEdit4(name, (float*)&style.Colors[i], true);
            if (memcmp(&style.Colors[i], (ref ? &ref->Colors[i] : &default_style.Colors[i]), sizeof(ImVec4)) != 0)
            {
                ImGui::SameLine(); if (ImGui::Button(XStr("Revert"))) style.Colors[i] = ref ? ref->Colors[i] : default_style.Colors[i];
                if (ref) { ImGui::SameLine(); if (ImGui::Button(XStr("Save"))) ref->Colors[i] = style.Colors[i]; }
            }
            ImGui::PopID();
        }
        ImGui::PopItemWidth();
        ImGui::EndChild();

        ImGui::TreePop();
    }

    if (ImGui::TreeNode(XStr("Fonts"), XStr("Fonts (%d)"), ImGui::GetIO().Fonts->Fonts.Size))
    {
        ImGui::SameLine(); ShowHelpMarker(XStr("Tip: Load fonts with io.Fonts->AddFontFromFileTTF()\nbefore calling io.Fonts->GetTex* functions."));
        ImFontAtlas* atlas = ImGui::GetIO().Fonts;
        if (ImGui::TreeNode(XStr("Atlas texture"), XStr("Atlas texture (%dx%d pixels)"), atlas->TexWidth, atlas->TexHeight))
        {
            ImGui::Image(atlas->TexID, ImVec2((float)atlas->TexWidth, (float)atlas->TexHeight), ImVec2(0,0), ImVec2(1,1), ImColor(255,255,255,255), ImColor(255,255,255,128));
            ImGui::TreePop();
        }
        ImGui::PushItemWidth(100);
        for (int i = 0; i < atlas->Fonts.Size; i++)
        {
            ImFont* font = atlas->Fonts[i];
            ImGui::BulletText(XStr("Font %d: \'%s\', %.2f px, %d glyphs"), i, font->ConfigData ? font->ConfigData[0].Name : XStr(""), font->FontSize, font->Glyphs.Size);
            ImGui::TreePush((void*)(intptr_t)i);
            ImGui::SameLine(); if (ImGui::SmallButton(XStr("Set as default"))) ImGui::GetIO().FontDefault = font;
            ImGui::PushFont(font);
            ImGui::Text(XStr("The quick brown fox jumps over the lazy dog"));
            ImGui::PopFont();
            if (ImGui::TreeNode(XStr("Details")))
            {
                ImGui::DragFloat(XStr("Font scale"), &font->Scale, 0.005f, 0.3f, 2.0f, XStr("%.1f"));   // Scale only this font
                ImGui::SameLine(); ShowHelpMarker(XStr("Note than the default embedded font is NOT meant to be scaled.\n\nFont are currently rendered into bitmaps at a given size at the time of building the atlas. You may oversample them to get some flexibility with scaling. You can also render at multiple sizes and select which one to use at runtime.\n\n(Glimmer of hope: the atlas system should hopefully be rewritten in the future to make scaling more natural and automatic.)"));
                ImGui::Text(XStr("Ascent: %f, Descent: %f, Height: %f"), font->Ascent, font->Descent, font->Ascent - font->Descent);
                ImGui::Text(XStr("Fallback character: '%c' (%d)"), font->FallbackChar, font->FallbackChar);
                ImGui::Text(XStr("Texture surface: %d pixels (approx) ~ %dx%d"), font->MetricsTotalSurface, (int)sqrtf((float)font->MetricsTotalSurface), (int)sqrtf((float)font->MetricsTotalSurface));
                for (int config_i = 0; config_i < font->ConfigDataCount; config_i++)
                {
                    ImFontConfig* cfg = &font->ConfigData[config_i];
                    ImGui::BulletText(XStr("Input %d: \'%s\', Oversample: (%d,%d), PixelSnapH: %d"), config_i, cfg->Name, cfg->OversampleH, cfg->OversampleV, cfg->PixelSnapH);
                }
                if (ImGui::TreeNode(XStr("Glyphs"), XStr("Glyphs (%d)"), font->Glyphs.Size))
                {
                    // Display all glyphs of the fonts in separate pages of 256 characters
                    const ImFont::Glyph* glyph_fallback = font->FallbackGlyph; // Forcefully/dodgily make FindGlyph() return NULL on fallback, which isn't the default behavior.
                    font->FallbackGlyph = NULL;
                    for (int base = 0; base < 0x10000; base += 256)
                    {
                        int count = 0;
                        for (int n = 0; n < 256; n++)
                            count += font->FindGlyph((ImWchar)(base + n)) ? 1 : 0;
                        if (count > 0 && ImGui::TreeNode((void*)(intptr_t)base, XStr("U+%04X..U+%04X (%d %s)"), base, base+255, count, count > 1 ? XStr("glyphs") : XStr("glyph")))
                        {
                            float cell_spacing = style.ItemSpacing.y;
                            ImVec2 cell_size(font->FontSize * 1, font->FontSize * 1);
                            ImVec2 base_pos = ImGui::GetCursorScreenPos();
                            ImDrawList* draw_list = ImGui::GetWindowDrawList();
                            for (int n = 0; n < 256; n++)
                            {
                                ImVec2 cell_p1(base_pos.x + (n % 16) * (cell_size.x + cell_spacing), base_pos.y + (n / 16) * (cell_size.y + cell_spacing));
                                ImVec2 cell_p2(cell_p1.x + cell_size.x, cell_p1.y + cell_size.y);
                                const ImFont::Glyph* glyph = font->FindGlyph((ImWchar)(base+n));;
                                draw_list->AddRect(cell_p1, cell_p2, glyph ? IM_COL32(255,255,255,100) : IM_COL32(255,255,255,50));
                                font->RenderChar(draw_list, cell_size.x, cell_p1, ImGui::GetColorU32(ImGuiCol_Text), (ImWchar)(base+n)); // We use ImFont::RenderChar as a shortcut because we don't have UTF-8 conversion functions available to generate a string.
                                if (glyph && ImGui::IsMouseHoveringRect(cell_p1, cell_p2))
                                {
                                    ImGui::BeginTooltip();
                                    ImGui::Text(XStr("Codepoint: U+%04X"), base+n);
                                    ImGui::Separator();
                                    ImGui::Text(XStr("XAdvance+1: %.1f"), glyph->XAdvance);
                                    ImGui::Text(XStr("Pos: (%.2f,%.2f)->(%.2f,%.2f)"), glyph->X0, glyph->Y0, glyph->X1, glyph->Y1);
                                    ImGui::Text(XStr("UV: (%.3f,%.3f)->(%.3f,%.3f)"), glyph->U0, glyph->V0, glyph->U1, glyph->V1);
                                    ImGui::EndTooltip();
                                }
                            }
                            ImGui::Dummy(ImVec2((cell_size.x + cell_spacing) * 16, (cell_size.y + cell_spacing) * 16));
                            ImGui::TreePop();
                        }
                    }
                    font->FallbackGlyph = glyph_fallback;
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        static float window_scale = 1.0f;
        ImGui::DragFloat(XStr("this window scale"), &window_scale, 0.005f, 0.3f, 2.0f, XStr("%.1f"));              // scale only this window
        ImGui::DragFloat(XStr("global scale"), &ImGui::GetIO().FontGlobalScale, 0.005f, 0.3f, 2.0f, XStr("%.1f")); // scale everything
        ImGui::PopItemWidth();
        ImGui::SetWindowFontScale(window_scale);
        ImGui::TreePop();
    }

    ImGui::PopItemWidth();
}

// Demonstrate creating a fullscreen menu bar and populating it.
static void ShowExampleAppMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu(XStr("File")))
        {
            ShowExampleMenuFile();
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu(XStr("Edit")))
        {
            if (ImGui::MenuItem(XStr("Undo"), XStr("CTRL+Z"))) {}
            if (ImGui::MenuItem(XStr("Redo"), XStr("CTRL+Y"), false, false)) {}  // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem(XStr("Cut"), XStr("CTRL+X"))) {}
            if (ImGui::MenuItem(XStr("Copy"), XStr("CTRL+C"))) {}
            if (ImGui::MenuItem(XStr("Paste"), XStr("CTRL+V"))) {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

static void ShowExampleMenuFile()
{
    ImGui::MenuItem(XStr("(dummy menu)"), NULL, false, false);
    if (ImGui::MenuItem(XStr("New"))) {}
    if (ImGui::MenuItem(XStr("Open"), XStr("Ctrl+O"))) {}
    if (ImGui::BeginMenu(XStr("Open Recent")))
    {
        ImGui::MenuItem(XStr("fish_hat.c"));
        ImGui::MenuItem(XStr("fish_hat.inl"));
        ImGui::MenuItem(XStr("fish_hat.h"));
        if (ImGui::BeginMenu(XStr("More..")))
        {
            ImGui::MenuItem(XStr("Hello"));
            ImGui::MenuItem(XStr("Sailor"));
            if (ImGui::BeginMenu(XStr("Recurse..")))
            {
                ShowExampleMenuFile();
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
    if (ImGui::MenuItem(XStr("Save"), XStr("Ctrl+S"))) {}
    if (ImGui::MenuItem(XStr("Save As.."))) {}
    ImGui::Separator();
    if (ImGui::BeginMenu(XStr("Options")))
    {
        static bool enabled = true;
        ImGui::MenuItem(XStr("Enabled"), XStr(""), &enabled);
        ImGui::BeginChild(XStr("child"), ImVec2(0, 60), true);
        for (int i = 0; i < 10; i++)
            ImGui::Text(XStr("Scrolling Text %d"), i);
        ImGui::EndChild();
        static float f = 0.5f;
        static int n = 0;
        ImGui::SliderFloat(XStr("Value"), &f, 0.0f, 1.0f);
        ImGui::InputFloat(XStr("Input"), &f, 0.1f);
        ImGui::Combo(XStr("Combo"), &n, XStr("Yes\0No\0Maybe\0\0"));
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu(XStr("Colors")))
    {
        for (int i = 0; i < ImGuiCol_COUNT; i++)
            ImGui::MenuItem(ImGui::GetStyleColName((ImGuiCol)i));
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu(XStr("Disabled"), false)) // Disabled
    {
        IM_ASSERT(0);
    }
    if (ImGui::MenuItem(XStr("Checked"), NULL, true)) {}
    if (ImGui::MenuItem(XStr("Quit"), XStr("Alt+F4"))) {}
}

// Demonstrate creating a window which gets auto-resized according to its content.
static void ShowExampleAppAutoResize(bool* p_open)
{
    if (!ImGui::Begin(XStr("Example: Auto-resizing window"), p_open, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::End();
        return;
    }

    static int lines = 10;
    ImGui::Text(XStr("Window will resize every-frame to the size of its content.\nNote that you probably don't want to query the window size to\noutput your content because that would create a feedback loop."));
    ImGui::SliderInt(XStr("Number of lines"), &lines, 1, 20);
    for (int i = 0; i < lines; i++)
        ImGui::Text(XStr("%*sThis is line %d"), i*4, XStr(""), i); // Pad with space to extend size horizontally
    ImGui::End();
}

// Demonstrate creating a window with custom resize constraints.
static void ShowExampleAppConstrainedResize(bool* p_open)
{
    struct CustomConstraints // Helper functions to demonstrate programmatic constraints
    {
        static void Square(ImGuiSizeConstraintCallbackData* data) { data->DesiredSize = ImVec2(IM_MAX(data->DesiredSize.x, data->DesiredSize.y), IM_MAX(data->DesiredSize.x, data->DesiredSize.y)); }
        static void Step(ImGuiSizeConstraintCallbackData* data)   { float step = (float)(int)(intptr_t)data->UserData; data->DesiredSize = ImVec2((int)(data->DesiredSize.x / step + 0.5f) * step, (int)(data->DesiredSize.y / step + 0.5f) * step); }
    };

    static int type = 0;
    if (type == 0) ImGui::SetNextWindowSizeConstraints(ImVec2(-1, 0),    ImVec2(-1, FLT_MAX));      // Vertical only
    if (type == 1) ImGui::SetNextWindowSizeConstraints(ImVec2(0, -1),    ImVec2(FLT_MAX, -1));      // Horizontal only
    if (type == 2) ImGui::SetNextWindowSizeConstraints(ImVec2(100, 100), ImVec2(FLT_MAX, FLT_MAX)); // Width > 100, Height > 100
    if (type == 3) ImGui::SetNextWindowSizeConstraints(ImVec2(300, 0),   ImVec2(400, FLT_MAX));     // Width 300-400
    if (type == 4) ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0),     ImVec2(FLT_MAX, FLT_MAX), CustomConstraints::Square);          // Always Square
    if (type == 5) ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0),     ImVec2(FLT_MAX, FLT_MAX), CustomConstraints::Step, (void*)100);// Fixed Step

    if (ImGui::Begin(XStr("Example: Constrained Resize"), p_open))
    {
        const char* desc[] = 
        {
            XStr("Resize vertical only"),
            XStr("Resize horizontal only"),
            XStr("Width > 100, Height > 100"),
            XStr("Width 300-400"),
            XStr("Custom: Always Square"),
            XStr("Custom: Fixed Steps (100)"),
        };
        ImGui::Combo(XStr("Constraint"), &type, desc, IM_ARRAYSIZE(desc)); 
        if (ImGui::Button(XStr("200x200"))) ImGui::SetWindowSize(ImVec2(200,200)); ImGui::SameLine();
        if (ImGui::Button(XStr("500x500"))) ImGui::SetWindowSize(ImVec2(500,500)); ImGui::SameLine();
        if (ImGui::Button(XStr("800x200"))) ImGui::SetWindowSize(ImVec2(800,200));
        for (int i = 0; i < 10; i++) 
            ImGui::Text(XStr("Hello, sailor! Making this line long enough for the example."));
    }
    ImGui::End();
}

// Demonstrate creating a simple static window with no decoration.
static void ShowExampleAppFixedOverlay(bool* p_open)
{
    ImGui::SetNextWindowPos(ImVec2(10,10));
    if (!ImGui::Begin(XStr("Example: Fixed Overlay"), p_open, ImVec2(0,0), 0.3f, ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings))
    {
        ImGui::End();
        return;
    }
    ImGui::Text(XStr("Simple overlay\non the top-left side of the screen."));
    ImGui::Separator();
    ImGui::Text(XStr("Mouse Position: (%.1f,%.1f)"), ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
    ImGui::End();
}

// Demonstrate using XStr("##") and XStr("###") in identifiers to manipulate ID generation.
// Read section XStr("How can I have multiple widgets with the same label? Can I have widget without a label? (Yes). A primer on the purpose of labels/IDs.") about ID.
static void ShowExampleAppManipulatingWindowTitle(bool*)
{
    // By default, Windows are uniquely identified by their title.
    // You can use the XStr("##") and XStr("###") markers to manipulate the display/ID.

    // Using XStr("##") to display same title but have unique identifier.
    ImGui::SetNextWindowPos(ImVec2(100,100), ImGuiSetCond_FirstUseEver);
    ImGui::Begin(XStr("Same title as another window##1"));
    ImGui::Text(XStr("This is window 1.\nMy title is the same as window 2, but my identifier is unique."));
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(100,200), ImGuiSetCond_FirstUseEver);
    ImGui::Begin(XStr("Same title as another window##2"));
    ImGui::Text(XStr("This is window 2.\nMy title is the same as window 1, but my identifier is unique."));
    ImGui::End();

    // Using XStr("###") to display a changing title but keep a static identifier XStr("AnimatedTitle")
    char buf[128];
    sprintf(buf, XStr("Animated title %c %d###AnimatedTitle"), XStr("|/-\\")[(int)(ImGui::GetTime()/0.25f)&3], rand());
    ImGui::SetNextWindowPos(ImVec2(100,300), ImGuiSetCond_FirstUseEver);
    ImGui::Begin(buf);
    ImGui::Text(XStr("This window has a changing title."));
    ImGui::End();
}

// Demonstrate using the low-level ImDrawList to draw custom shapes. 
static void ShowExampleAppCustomRendering(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(350,560), ImGuiSetCond_FirstUseEver);
    if (!ImGui::Begin(XStr("Example: Custom rendering"), p_open))
    {
        ImGui::End();
        return;
    }

    // Tip: If you do a lot of custom rendering, you probably want to use your own geometrical types and benefit of overloaded operators, etc.
    // Define IM_VEC2_CLASS_EXTRA in imconfig.h to create implicit conversions between your types and ImVec2/ImVec4.
    // ImGui defines overloaded operators but they are internal to imgui.cpp and not exposed outside (to avoid messing with your types)
    // In this example we are not using the maths operators!
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Primitives
    ImGui::Text(XStr("Primitives"));
    static float sz = 36.0f;
    static ImVec4 col = ImVec4(1.0f,1.0f,0.4f,1.0f);
    ImGui::DragFloat(XStr("Size"), &sz, 0.2f, 2.0f, 72.0f, XStr("%.0f"));
    ImGui::ColorEdit3(XStr("Color"), &col.x);
    {
        const ImVec2 p = ImGui::GetCursorScreenPos();
        const ImU32 col32 = ImColor(col);
        float x = p.x + 4.0f, y = p.y + 4.0f, spacing = 8.0f;
        for (int n = 0; n < 2; n++)
        {
            float thickness = (n == 0) ? 1.0f : 4.0f;
            draw_list->AddCircle(ImVec2(x+sz*0.5f, y+sz*0.5f), sz*0.5f, col32, 20, thickness); x += sz+spacing;
            draw_list->AddRect(ImVec2(x, y), ImVec2(x+sz, y+sz), col32, 0.0f, ~0, thickness); x += sz+spacing;
            draw_list->AddRect(ImVec2(x, y), ImVec2(x+sz, y+sz), col32, 10.0f, ~0, thickness); x += sz+spacing;
            draw_list->AddTriangle(ImVec2(x+sz*0.5f, y), ImVec2(x+sz,y+sz-0.5f), ImVec2(x,y+sz-0.5f), col32, thickness); x += sz+spacing;
            draw_list->AddLine(ImVec2(x, y), ImVec2(x+sz, y   ), col32, thickness); x += sz+spacing;
            draw_list->AddLine(ImVec2(x, y), ImVec2(x+sz, y+sz), col32, thickness); x += sz+spacing;
            draw_list->AddLine(ImVec2(x, y), ImVec2(x,    y+sz), col32, thickness); x += spacing;
            draw_list->AddBezierCurve(ImVec2(x, y), ImVec2(x+sz*1.3f,y+sz*0.3f), ImVec2(x+sz-sz*1.3f,y+sz-sz*0.3f), ImVec2(x+sz, y+sz), col32, thickness);
            x = p.x + 4;
            y += sz+spacing;
        }
        draw_list->AddCircleFilled(ImVec2(x+sz*0.5f, y+sz*0.5f), sz*0.5f, col32, 32); x += sz+spacing;
        draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x+sz, y+sz), col32); x += sz+spacing;
        draw_list->AddRectFilled(ImVec2(x, y), ImVec2(x+sz, y+sz), col32, 10.0f); x += sz+spacing;
        draw_list->AddTriangleFilled(ImVec2(x+sz*0.5f, y), ImVec2(x+sz,y+sz-0.5f), ImVec2(x,y+sz-0.5f), col32); x += sz+spacing;
        draw_list->AddRectFilledMultiColor(ImVec2(x, y), ImVec2(x+sz, y+sz), ImColor(0,0,0), ImColor(255,0,0), ImColor(255,255,0), ImColor(0,255,0));
        ImGui::Dummy(ImVec2((sz+spacing)*8, (sz+spacing)*3));
    }
    ImGui::Separator();
    {
        static ImVector<ImVec2> points;
        static bool adding_line = false;
        ImGui::Text(XStr("Canvas example"));
        if (ImGui::Button(XStr("Clear"))) points.clear();
        if (points.Size >= 2) { ImGui::SameLine(); if (ImGui::Button(XStr("Undo"))) { points.pop_back(); points.pop_back(); } }
        ImGui::Text(XStr("Left-click and drag to add lines,\nRight-click to undo"));

        // Here we are using InvisibleButton() as a convenience to 1) advance the cursor and 2) allows us to use IsItemHovered()
        // However you can draw directly and poll mouse/keyboard by yourself. You can manipulate the cursor using GetCursorPos() and SetCursorPos().
        // If you only use the ImDrawList API, you can notify the owner window of its extends by using SetCursorPos(max).
        ImVec2 canvas_pos = ImGui::GetCursorScreenPos();            // ImDrawList API uses screen coordinates!
        ImVec2 canvas_size = ImGui::GetContentRegionAvail();        // Resize canvas to what's available
        if (canvas_size.x < 50.0f) canvas_size.x = 50.0f;
        if (canvas_size.y < 50.0f) canvas_size.y = 50.0f;
        draw_list->AddRectFilledMultiColor(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), ImColor(50,50,50), ImColor(50,50,60), ImColor(60,60,70), ImColor(50,50,60));
        draw_list->AddRect(canvas_pos, ImVec2(canvas_pos.x + canvas_size.x, canvas_pos.y + canvas_size.y), ImColor(255,255,255));

        bool adding_preview = false;
        ImGui::InvisibleButton(XStr("canvas"), canvas_size);
        ImVec2 mouse_pos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - canvas_pos.x, ImGui::GetIO().MousePos.y - canvas_pos.y);
        if (adding_line)
        {
            adding_preview = true;
            points.push_back(mouse_pos_in_canvas);
            if (!ImGui::GetIO().MouseDown[0])
                adding_line = adding_preview = false;
        }
        if (ImGui::IsItemHovered())
        {
            if (!adding_line && ImGui::IsMouseClicked(0))
            {
                points.push_back(mouse_pos_in_canvas);
                adding_line = true;
            }
            if (ImGui::IsMouseClicked(1) && !points.empty())
            {
                adding_line = adding_preview = false;
                points.pop_back();
                points.pop_back();
            }
        }
        draw_list->PushClipRect(canvas_pos, ImVec2(canvas_pos.x+canvas_size.x, canvas_pos.y+canvas_size.y));      // clip lines within the canvas (if we resize it, etc.)
        for (int i = 0; i < points.Size - 1; i += 2)
            draw_list->AddLine(ImVec2(canvas_pos.x + points[i].x, canvas_pos.y + points[i].y), ImVec2(canvas_pos.x + points[i+1].x, canvas_pos.y + points[i+1].y), IM_COL32(255,255,0,255), 2.0f);
        draw_list->PopClipRect();
        if (adding_preview)
            points.pop_back();
    }
    ImGui::End();
}

// Demonstrating creating a simple console window, with scrolling, filtering, completion and history.
// For the console example, here we are using a more C++ like approach of declaring a class to hold the data and the functions.
struct ExampleAppConsole
{
    char                  InputBuf[256];
    ImVector<char*>       Items;
    bool                  ScrollToBottom;
    ImVector<char*>       History;
    int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
    ImVector<const char*> Commands;

    ExampleAppConsole()
    {
        ClearLog();
        memset(InputBuf, 0, sizeof(InputBuf));
        HistoryPos = -1;
        Commands.push_back(XStr("HELP"));
        Commands.push_back(XStr("HISTORY"));
        Commands.push_back(XStr("CLEAR"));
        Commands.push_back(XStr("CLASSIFY"));  // XStr("classify") is here to provide an example of XStr("C")+[tab] completing to XStr("CL") and displaying matches.
        AddLog(XStr("Welcome to ImGui!"));
    }
    ~ExampleAppConsole()
    {
        ClearLog();
        for (int i = 0; i < History.Size; i++)
            free(History[i]);
    }

    // Portable helpers
    static int   Stricmp(const char* str1, const char* str2)         { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
    static int   Strnicmp(const char* str1, const char* str2, int n) { int d = 0; while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; n--; } return d; }
    static char* Strdup(const char *str)                             { size_t len = strlen(str) + 1; void* buff = malloc(len); return (char*)memcpy(buff, (const void*)str, len); }

    void    ClearLog()
    {
        for (int i = 0; i < Items.Size; i++)
            free(Items[i]);
        Items.clear();
        ScrollToBottom = true;
    }

    void    AddLog(const char* fmt, ...) IM_PRINTFARGS(2)
    {
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf)-1] = 0;
        va_end(args);
        Items.push_back(Strdup(buf));
        ScrollToBottom = true;
    }

    void    Draw(const char* title, bool* p_open)
    {
        ImGui::SetNextWindowSize(ImVec2(520,600), ImGuiSetCond_FirstUseEver);
        if (!ImGui::Begin(title, p_open))
        {
            ImGui::End();
            return;
        }

        ImGui::TextWrapped(XStr("This example implements a console with basic coloring, completion and history. A more elaborate implementation may want to store entries along with extra data such as timestamp, emitter, etc."));
        ImGui::TextWrapped(XStr("Enter 'HELP' for help, press TAB to use text completion."));

        // TODO: display items starting from the bottom

        if (ImGui::SmallButton(XStr("Add Dummy Text"))) { AddLog(XStr("%d some text"), Items.Size); AddLog(XStr("some more text")); AddLog(XStr("display very important message here!")); } ImGui::SameLine();
        if (ImGui::SmallButton(XStr("Add Dummy Error"))) AddLog(XStr("[error] something went wrong")); ImGui::SameLine();
        if (ImGui::SmallButton(XStr("Clear"))) ClearLog(); ImGui::SameLine();
        if (ImGui::SmallButton(XStr("Scroll to bottom"))) ScrollToBottom = true;
        //static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog(XStr("Spam %f"), t); }

        ImGui::Separator();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0,0));
        static ImGuiTextFilter filter;
        filter.Draw(XStr("Filter (\"incl,-excl\") (\"error\")"), 180);
        ImGui::PopStyleVar();
        ImGui::Separator();

        ImGui::BeginChild(XStr("ScrollingRegion"), ImVec2(0,-ImGui::GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::Selectable(XStr("Clear"))) ClearLog();
            ImGui::EndPopup();
        }

        // Display every line as a separate entry so we can change their color or add custom widgets. If you only want raw text you can use ImGui::TextUnformatted(log.begin(), log.end());
        // NB- if you have thousands of entries this approach may be too inefficient and may require user-side clipping to only process visible items.
        // You can seek and display only the lines that are visible using the ImGuiListClipper helper, if your elements are evenly spaced and you have cheap random access to the elements.
        // To use the clipper we could replace the 'for (int i = 0; i < Items.Size; i++)' loop with:
        //     ImGuiListClipper clipper(Items.Size);
        //     while (clipper.Step())
        //         for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
        // However take note that you can not use this code as is if a filter is active because it breaks the 'cheap random-access' property. We would need random-access on the post-filtered list.
        // A typical application wanting coarse clipping and filtering may want to pre-compute an array of indices that passed the filtering test, recomputing this array when user changes the filter,
        // and appending newly elements as they are inserted. This is left as a task to the user until we can manage to improve this example code!
        // If your items are of variable size you may want to implement code similar to what ImGuiListClipper does. Or split your data into fixed height items to allow random-seeking into your list.
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4,1)); // Tighten spacing
        for (int i = 0; i < Items.Size; i++)
        {
            const char* item = Items[i];
            if (!filter.PassFilter(item))
                continue;
            ImVec4 col = ImVec4(1.0f,1.0f,1.0f,1.0f); // A better implementation may store a type per-item. For the sample let's just parse the text.
            if (strstr(item, XStr("[error]"))) col = ImColor(1.0f,0.4f,0.4f,1.0f);
            else if (strncmp(item, XStr("# "), 2) == 0) col = ImColor(1.0f,0.78f,0.58f,1.0f);
            ImGui::PushStyleColor(ImGuiCol_Text, col);
            ImGui::TextUnformatted(item);
            ImGui::PopStyleColor();
        }
        if (ScrollToBottom)
            ImGui::SetScrollHere();
        ScrollToBottom = false;
        ImGui::PopStyleVar();
        ImGui::EndChild();
        ImGui::Separator();

        // Command-line
        if (ImGui::InputText(XStr("Input"), InputBuf, IM_ARRAYSIZE(InputBuf), ImGuiInputTextFlags_EnterReturnsTrue|ImGuiInputTextFlags_CallbackCompletion|ImGuiInputTextFlags_CallbackHistory, &TextEditCallbackStub, (void*)this))
        {
            char* input_end = InputBuf+strlen(InputBuf);
            while (input_end > InputBuf && input_end[-1] == ' ') input_end--; *input_end = 0;
            if (InputBuf[0])
                ExecCommand(InputBuf);
            strcpy(InputBuf, XStr(""));
        }

        // Demonstrate keeping auto focus on the input box
        if (ImGui::IsItemHovered() || (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)))
            ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

        ImGui::End();
    }

    void    ExecCommand(const char* command_line)
    {
        AddLog(XStr("# %s\n"), command_line);

        // Insert into history. First find match and delete it so it can be pushed to the back. This isn't trying to be smart or optimal.
        HistoryPos = -1;
        for (int i = History.Size-1; i >= 0; i--)
            if (Stricmp(History[i], command_line) == 0)
            {
                free(History[i]);
                History.erase(History.begin() + i);
                break;
            }
        History.push_back(Strdup(command_line));

        // Process command
        if (Stricmp(command_line, XStr("CLEAR")) == 0)
        {
            ClearLog();
        }
        else if (Stricmp(command_line, XStr("HELP")) == 0)
        {
            AddLog(XStr("Commands:"));
            for (int i = 0; i < Commands.Size; i++)
                AddLog(XStr("- %s"), Commands[i]);
        }
        else if (Stricmp(command_line, XStr("HISTORY")) == 0)
        {
            for (int i = History.Size >= 10 ? History.Size - 10 : 0; i < History.Size; i++)
                AddLog(XStr("%3d: %s\n"), i, History[i]);
        }
        else
        {
            AddLog(XStr("Unknown command: '%s'\n"), command_line);
        }
    }

    static int TextEditCallbackStub(ImGuiTextEditCallbackData* data) // In C++11 you are better off using lambdas for this sort of forwarding callbacks
    {
        ExampleAppConsole* console = (ExampleAppConsole*)data->UserData;
        return console->TextEditCallback(data);
    }

    int     TextEditCallback(ImGuiTextEditCallbackData* data)
    {
        //AddLog(XStr("cursor: %d, selection: %d-%d"), data->CursorPos, data->SelectionStart, data->SelectionEnd);
        switch (data->EventFlag)
        {
        case ImGuiInputTextFlags_CallbackCompletion:
            {
                // Example of TEXT COMPLETION

                // Locate beginning of current word
                const char* word_end = data->Buf + data->CursorPos;
                const char* word_start = word_end;
                while (word_start > data->Buf)
                {
                    const char c = word_start[-1];
                    if (c == ' ' || c == '\t' || c == ',' || c == ';')
                        break;
                    word_start--;
                }

                // Build a list of candidates
                ImVector<const char*> candidates;
                for (int i = 0; i < Commands.Size; i++)
                    if (Strnicmp(Commands[i], word_start, (int)(word_end-word_start)) == 0)
                        candidates.push_back(Commands[i]);

                if (candidates.Size == 0)
                {
                    // No match
                    AddLog(XStr("No match for \"%.*s\"!\n"), (int)(word_end-word_start), word_start);
                }
                else if (candidates.Size == 1)
                {
                    // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
                    data->DeleteChars((int)(word_start-data->Buf), (int)(word_end-word_start));
                    data->InsertChars(data->CursorPos, candidates[0]);
                    data->InsertChars(data->CursorPos, XStr(" "));
                }
                else
                {
                    // Multiple matches. Complete as much as we can, so inputing XStr("C") will complete to XStr("CL") and display XStr("CLEAR") and XStr("CLASSIFY")
                    int match_len = (int)(word_end - word_start);
                    for (;;)
                    {
                        int c = 0;
                        bool all_candidates_matches = true;
                        for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
                            if (i == 0)
                                c = toupper(candidates[i][match_len]);
                            else if (c == 0 || c != toupper(candidates[i][match_len]))
                                all_candidates_matches = false;
                        if (!all_candidates_matches)
                            break;
                        match_len++;
                    }

                    if (match_len > 0)
                    {
                        data->DeleteChars((int)(word_start - data->Buf), (int)(word_end-word_start));
                        data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
                    }

                    // List matches
                    AddLog(XStr("Possible matches:\n"));
                    for (int i = 0; i < candidates.Size; i++)
                        AddLog(XStr("- %s\n"), candidates[i]);
                }

                break;
            }
        case ImGuiInputTextFlags_CallbackHistory:
            {
                // Example of HISTORY
                const int prev_history_pos = HistoryPos;
                if (data->EventKey == ImGuiKey_UpArrow)
                {
                    if (HistoryPos == -1)
                        HistoryPos = History.Size - 1;
                    else if (HistoryPos > 0)
                        HistoryPos--;
                }
                else if (data->EventKey == ImGuiKey_DownArrow)
                {
                    if (HistoryPos != -1)
                        if (++HistoryPos >= History.Size)
                            HistoryPos = -1;
                }

                // A better implementation would preserve the data on the current input line along with cursor position.
                if (prev_history_pos != HistoryPos)
                {
                    data->CursorPos = data->SelectionStart = data->SelectionEnd = data->BufTextLen = (int)snprintf(data->Buf, (size_t)data->BufSize, XStr("%s"), (HistoryPos >= 0) ? History[HistoryPos] : XStr(""));
                    data->BufDirty = true;
                }
            }
        }
        return 0;
    }
};

static void ShowExampleAppConsole(bool* p_open)
{
    static ExampleAppConsole console;
    console.Draw(XStr("Example: Console"), p_open);
}

// Usage:
//  static ExampleAppLog my_log;
//  my_log.AddLog(XStr("Hello %d world\n"), 123);
//  my_log.Draw(XStr("title"));
struct ExampleAppLog
{
    ImGuiTextBuffer     Buf;
    ImGuiTextFilter     Filter;
    ImVector<int>       LineOffsets;        // Index to lines offset
    bool                ScrollToBottom;

    void    Clear()     { Buf.clear(); LineOffsets.clear(); }

    void    AddLog(const char* fmt, ...) IM_PRINTFARGS(2)
    {
        int old_size = Buf.size();
        va_list args;
        va_start(args, fmt);
        Buf.appendv(fmt, args);
        va_end(args);
        for (int new_size = Buf.size(); old_size < new_size; old_size++)
            if (Buf[old_size] == '\n')
                LineOffsets.push_back(old_size);
        ScrollToBottom = true;
    }

    void    Draw(const char* title, bool* p_open = NULL)
    {
        ImGui::SetNextWindowSize(ImVec2(500,400), ImGuiSetCond_FirstUseEver);
        ImGui::Begin(title, p_open);
        if (ImGui::Button(XStr("Clear"))) Clear();
        ImGui::SameLine();
        bool copy = ImGui::Button(XStr("Copy"));
        ImGui::SameLine();
        Filter.Draw(XStr("Filter"), -100.0f);
        ImGui::Separator();
        ImGui::BeginChild(XStr("scrolling"), ImVec2(0,0), false, ImGuiWindowFlags_HorizontalScrollbar);
        if (copy) ImGui::LogToClipboard();

        if (Filter.IsActive())
        {
            const char* buf_begin = Buf.begin();
            const char* line = buf_begin;
            for (int line_no = 0; line != NULL; line_no++)
            {
                const char* line_end = (line_no < LineOffsets.Size) ? buf_begin + LineOffsets[line_no] : NULL;
                if (Filter.PassFilter(line, line_end))
                    ImGui::TextUnformatted(line, line_end);
                line = line_end && line_end[1] ? line_end + 1 : NULL;
            }
        }
        else
        {
            ImGui::TextUnformatted(Buf.begin());
        }

        if (ScrollToBottom)
            ImGui::SetScrollHere(1.0f);
        ScrollToBottom = false;
        ImGui::EndChild();
        ImGui::End();
    }
};

// Demonstrate creating a simple log window with basic filtering.
static void ShowExampleAppLog(bool* p_open)
{
    static ExampleAppLog log;

    // Demo fill
    static float last_time = -1.0f;
    float time = ImGui::GetTime();
    if (time - last_time >= 0.3f)
    {
        const char* random_words[] = { XStr("system"), XStr("info"), XStr("warning"), XStr("error"), XStr("fatal"), XStr("notice"), XStr("log") };
        log.AddLog(XStr("[%s] Hello, time is %.1f, rand() %d\n"), random_words[rand() % IM_ARRAYSIZE(random_words)], time, (int)rand());
        last_time = time;
    }

    log.Draw(XStr("Example: Log"), p_open);
}

// Demonstrate create a window with multiple child windows.
static void ShowExampleAppLayout(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiSetCond_FirstUseEver);
    if (ImGui::Begin(XStr("Example: Layout"), p_open, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu(XStr("File")))
            {
                if (ImGui::MenuItem(XStr("Close"))) *p_open = false;
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // left
        static int selected = 0;
        ImGui::BeginChild(XStr("left pane"), ImVec2(150, 0), true);
        for (int i = 0; i < 100; i++)
        {
            char label[128];
            sprintf(label, XStr("MyObject %d"), i);
            if (ImGui::Selectable(label, selected == i))
                selected = i;
        }
        ImGui::EndChild();
        ImGui::SameLine();

        // right
        ImGui::BeginGroup();
            ImGui::BeginChild(XStr("item view"), ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing())); // Leave room for 1 line below us
                ImGui::Text(XStr("MyObject: %d"), selected);
                ImGui::Separator();
                ImGui::TextWrapped(XStr("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. "));
            ImGui::EndChild();
            ImGui::BeginChild(XStr("buttons"));
                if (ImGui::Button(XStr("Revert"))) {}
                ImGui::SameLine();
                if (ImGui::Button(XStr("Save"))) {}
            ImGui::EndChild();
        ImGui::EndGroup();
    }
    ImGui::End();
}

// Demonstrate create a simple property editor.
static void ShowExampleAppPropertyEditor(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(430,450), ImGuiSetCond_FirstUseEver);
    if (!ImGui::Begin(XStr("Example: Property editor"), p_open))
    {
        ImGui::End();
        return;
    }

    ShowHelpMarker(XStr("This example shows how you may implement a property editor using two columns.\nAll objects/fields data are dummies here.\nRemember that in many simple cases, you can use ImGui::SameLine(xxx) to position\nyour cursor horizontally instead of using the Columns() API."));

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2,2));
    ImGui::Columns(2);
    ImGui::Separator();

    struct funcs
    {
        static void ShowDummyObject(const char* prefix, int uid)
        {
            ImGui::PushID(uid);                      // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
            ImGui::AlignFirstTextHeightToWidgets();  // Text and Tree nodes are less high than regular widgets, here we add vertical spacing to make the tree lines equal high.
            bool node_open = ImGui::TreeNode(XStr("Object"), XStr("%s_%u"), prefix, uid);
            ImGui::NextColumn();
            ImGui::AlignFirstTextHeightToWidgets();
            ImGui::Text(XStr("my sailor is rich"));
            ImGui::NextColumn();
            if (node_open)
            {
                static float dummy_members[8] = { 0.0f,0.0f,1.0f,3.1416f,100.0f,999.0f };
                for (int i = 0; i < 8; i++)
                {
                    ImGui::PushID(i); // Use field index as identifier.
                    if (i < 2)
                    {
                        ShowDummyObject(XStr("Child"), 424242);
                    }
                    else
                    {
                        ImGui::AlignFirstTextHeightToWidgets();
                        // Here we use a Selectable (instead of Text) to highlight on hover
                        //ImGui::Text(XStr("Field_%d"), i);
                        char label[32];
                        sprintf(label, XStr("Field_%d"), i);
                        ImGui::Bullet();
                        ImGui::Selectable(label);
                        ImGui::NextColumn();
                        ImGui::PushItemWidth(-1);
                        if (i >= 5)
                            ImGui::InputFloat(XStr("##value"), &dummy_members[i], 1.0f);
                        else
                            ImGui::DragFloat(XStr("##value"), &dummy_members[i], 0.01f);
                        ImGui::PopItemWidth();
                        ImGui::NextColumn();
                    }
                    ImGui::PopID();
                }
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
    };

    // Iterate dummy objects with dummy members (all the same data)
    for (int obj_i = 0; obj_i < 3; obj_i++)
        funcs::ShowDummyObject(XStr("Object"), obj_i);

    ImGui::Columns(1);
    ImGui::Separator();
    ImGui::PopStyleVar();
    ImGui::End();
}

// Demonstrate/test rendering huge amount of text, and the incidence of clipping.
static void ShowExampleAppLongText(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(520,600), ImGuiSetCond_FirstUseEver);
    if (!ImGui::Begin(XStr("Example: Long text display"), p_open))
    {
        ImGui::End();
        return;
    }

    static int test_type = 0;
    static ImGuiTextBuffer log;
    static int lines = 0;
    ImGui::Text(XStr("Printing unusually long amount of text."));
    ImGui::Combo(XStr("Test type"), &test_type, XStr("Single call to TextUnformatted()\0Multiple calls to Text(), clipped manually\0Multiple calls to Text(), not clipped\0"));
    ImGui::Text(XStr("Buffer contents: %d lines, %d bytes"), lines, log.size());
    if (ImGui::Button(XStr("Clear"))) { log.clear(); lines = 0; }
    ImGui::SameLine();
    if (ImGui::Button(XStr("Add 1000 lines")))
    {
        for (int i = 0; i < 1000; i++)
            log.append(XStr("%i The quick brown fox jumps over the lazy dog\n"), lines+i);
        lines += 1000;
    }
    ImGui::BeginChild(XStr("Log"));
    switch (test_type)
    {
    case 0:
        // Single call to TextUnformatted() with a big buffer
        ImGui::TextUnformatted(log.begin(), log.end());
        break;
    case 1:
        {
            // Multiple calls to Text(), manually coarsely clipped - demonstrate how to use the ImGuiListClipper helper.
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));
            ImGuiListClipper clipper(lines);
            while (clipper.Step())
                for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                    ImGui::Text(XStr("%i The quick brown fox jumps over the lazy dog"), i);
            ImGui::PopStyleVar();
            break;
        }
    case 2:
        // Multiple calls to Text(), not clipped (slow)
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0,0));
        for (int i = 0; i < lines; i++)
            ImGui::Text(XStr("%i The quick brown fox jumps over the lazy dog"), i);
        ImGui::PopStyleVar();
        break;
    }
    ImGui::EndChild();
    ImGui::End();
}

// End of Demo code
#else

void ImGui::ShowTestWindow(bool*) {}
void ImGui::ShowUserGuide() {}
void ImGui::ShowStyleEditor(ImGuiStyle*) {}

#endif
