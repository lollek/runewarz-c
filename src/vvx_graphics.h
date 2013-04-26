#ifndef VVK_GRAPHICS_H
#define VVK_GRAPHICS_H

#include <stdio.h>

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"

#include "vvl_link.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_BPP 32
#define TILESIZE 15
#define SYMBOL_OFFSET 15

#define CAPTION "RuneWarz v0.1.2d"

int vvx_init(Master* master);
void vvx_exit(Master* master);

void vvk_render_text_centered(SDL_Surface** target_surface, TTF_Font** font, const char text[],
                              SDL_Color text_color, int offset_x, int offset_y);
void vvk_render_text_absolute(SDL_Surface** target_surface, TTF_Font** font, const char text[],
                              SDL_Color text_color, int offset_x, int offset_y);
void vvk_render_box_centered(SDL_Surface** target_surface,
                             int offset_x, int offset_y, int width, int height);
void vvk_render_box_absolute(SDL_Surface** target_surface,
                             int offset_x, int offset_y, int width, int height);

void vvx_draw_all_caps(Master* master);
void vvx_draw_capture(Master* master);
void vvx_draw_hoverlist(Master* master, int pad);
void vvx_hide_hoverlist(Master* master);
#endif
