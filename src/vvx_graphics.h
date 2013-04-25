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

#define CAPTION "RuneWarz v0.1"

int vvx_init(SDL_Surface** stdscr, SDL_Surface** imgscr);
void vvx_exit(SDL_Surface** imgscr);
void vvk_render_text_centered(SDL_Surface** target_surface, TTF_Font** font, const char text[],
                              SDL_Color text_color, int offset_x, int offset_y);
void vvk_render_text_absolute(SDL_Surface** target_surface, TTF_Font** font, const char text[],
                              SDL_Color text_color, int offset_x, int offset_y);
void vvk_render_box_centered(SDL_Surface** target_surface,
                             int offset_x, int offset_y, int width, int height);
void vvk_render_box_absolute(SDL_Surface** target_surface,
                             int offset_x, int offset_y, int width, int height);
void vvk_draw_all_caps(SDL_Surface** stdscr, SDL_Surface** imgscr,
                       Cap** cap_root, Player** player_root);
void vvk_draw_capture(SDL_Surface** stdscr, SDL_Surface** imgscr,
                      Player** player_root);
void vvk_draw_hoverlist(SDL_Surface** stdscr, SDL_Surface** imgscr,
                        Player** player_root);
void vvk_draw_hoverlist_blank(SDL_Surface** stdscr, SDL_Surface** imgscr,
                              Player** player_root);
#endif
