/*
 * drawex.cpp
 *
 *  Created on: Nov 12, 2017
 *      Author: nullifiedcat
 */

#include "common.hpp"
#include "hack.hpp"

#include <SDL2/SDL.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <visual/SDLHooks.hpp>

#if EXTERNAL_DRAWING
extern "C" {
#include <xoverlay.h>
}
#endif

SDL_GLContext context = nullptr;

namespace drawex
{

namespace api
{

bool ready_state = false;
bool init        = false;

bool ready()
{
    return ready_state && init;
}

void initialize()
{
#if EXTERNAL_DRAWING
    int status = xoverlay_init();
    xoverlay_draw_begin();
    glez_init(xoverlay_library.width, xoverlay_library.height);
    xoverlay_draw_end();
    if (status < 0)
    {
        logging::Info("ERROR: could not initialize Xoverlay: %d", status);
    }
    else
    {
        logging::Info("Xoverlay initialized");
    }
    xoverlay_show();
    context = SDL_GL_CreateContext(sdl_hooks::window);
#else
    glClearColor(1.0, 0.0, 0.0, 0.5);
    glewExperimental = GL_TRUE;
    glewInit();
    glez::init(draw::width, draw::height);
#endif
}

void draw_rect(float x, float y, float w, float h, const rgba_t &rgba)
{
    glez::draw::rect(x, y, w, h, *reinterpret_cast<const glez::rgba *>(&rgba));
}

void draw_rect_outlined(float x, float y, float w, float h, const rgba_t &rgba,
                        float thickness)
{
    glez::draw::rect_outline(x, y, w, h, *reinterpret_cast<const glez::rgba *>(&rgba), thickness);
}

void draw_line(float x, float y, float dx, float dy, const rgba_t &rgba,
               float thickness)
{
    glez::draw::line(x, y, dx, dy, *reinterpret_cast<const glez::rgba *>(&rgba), thickness);
}

void draw_rect_textured(float x, float y, float w, float h, const rgba_t &rgba,
                        glez::texture& texture, float u, float v, float s,
                        float t, float a)
{
    glez::draw::rect_textured(x, y, w, h, *reinterpret_cast<const glez::rgba *>(&rgba), texture, u, v, s, t, a);
}

void draw_circle(float x, float y, float radius, const rgba_t &rgba,
                 float thickness, int steps)
{
    glez::draw::circle(x, y, radius, *reinterpret_cast<const glez::rgba *>(&rgba), thickness, steps);
}

void draw_string(float x, float y, const char *string, glez::font &font,
                 const rgba_t &rgba)
{
    glez::draw::string(x, y, string, font, *reinterpret_cast<const glez::rgba *>(&rgba), nullptr, nullptr);
}

void draw_string_with_outline(float x, float y, const char *string,
                              glez::font &font, const rgba_t &rgba,
                              const rgba_t &rgba_outline, float thickness)
{
    glez::draw::outlined_string(x, y, string, font, *reinterpret_cast<const glez::rgba *>(&rgba), *reinterpret_cast<const glez::rgba *>(&rgba_outline), thickness, nullptr, nullptr);
}

void get_string_size(const char *string, glez::font &font, float *x,
                     float *y)
{
    font.getStringSize(string, x, y);
}

void draw_begin()
{
    PROF_SECTION(DRAWEX_draw_begin);
#if EXTERNAL_DRAWING
    xoverlay_draw_begin();
    {
        PROF_SECTION(draw_begin__SDL_GL_MakeCurrent);
       // SDL_GL_MakeCurrent(sdl_hooks::window, context);
    }
#endif
    {
        glActiveTexture(GL_TEXTURE0);
        PROF_SECTION(draw_begin__glez_end)
        glez::begin();
    }
}

void draw_end()
{
    PROF_SECTION(DRAWEX_draw_end);
    {
        PROF_SECTION(draw_end__glez_end);
        glez::end();
    }
#if EXTERNAL_DRAWING
    xoverlay_draw_end();
    {
        PROF_SECTION(draw_end__SDL_GL_MakeCurrent);
        SDL_GL_MakeCurrent(sdl_hooks::window, nullptr);
    }
#endif
}
}
}
