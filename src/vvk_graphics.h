#ifndef VVK_GRAPHICS_H
#define VVK_GRAPHICS_H

#include <stdio.h>

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"

#include "vvk_game.h"

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
