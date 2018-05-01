/*
 * drawex.hpp
 *
 *  Created on: Nov 12, 2017
 *      Author: nullifiedcat
 */

#pragma once

#include <thread>

#include <glez/glez.hpp>
extern "C" {
#if EXTERNAL_DRAWING
#include <xoverlay.h>
#endif
}

#define draw_api drawex::api

namespace drawex
{

namespace api
{

bool ready();

void initialize();

void draw_rect(float x, float y, float w, float h, const rgba_t &rgba);
void draw_rect_outlined(float x, float y, float w, float h, const rgba_t &rgba,
                        float thickness);
void draw_line(float x, float y, float dx, float dy, const rgba_t &rgba,
               float thickness);
void draw_rect_textured(float x, float y, float w, float h, const rgba_t &rgba,
                        glez::texture& texture, float u, float v, float s,
                        float t, float a);
void draw_circle(float x, float y, float radius, const rgba_t &rgba,
                 float thickness, int steps);
void draw_string(float x, float y, const char *string, glez::font& font,
                 const rgba_t &rgba);
void draw_string_with_outline(float x, float y, const char *string,
                              glez::font& font, const rgba_t &rgba,
                              const rgba_t &rgba_outline, float thickness);
void get_string_size(const char *string, glez::font& font, float *x,
                     float *y);

void draw_begin();
void draw_end();
}
}
