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

#define CAPTION "RuneWarz v0.2"

int vvx_init(Master* master);
void vvx_exit(Master* master);

void vvx_draw_main_menu(Master* master, Map* map_root, char highmap);
void vvx_draw_text(Master* master, const char text[], int x, int y, int is_c, int clr);

void vvx_draw_all_caps(Master* master);
void vvx_draw_capture(Master* master);
void vvx_draw_hoverlist(Master* master, int pad);
void vvx_hide_hoverlist(Master* master);
#endif
