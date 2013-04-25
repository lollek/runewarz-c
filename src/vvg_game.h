#ifndef VVK_GAME_H
#define VVK_GAME_H

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "SDL/SDL.h"

#include "vvl_link.h"
#include "vvx_graphics.h"

int vvg_get_filesize(FILE* fp);
int vvg_load_mapfile(const char* mapname, char** buf);
int vvg_make_map(char** map_buffer, Cap** cap_root, Player** player_root, int* instances);
void vvg_free_map(Cap** cap_root, Player** player_root);

int vvg_play_game(SDL_Surface** stdscr, SDL_Surface** imgscr,
                  Cap** cap_root, Player** player_root, int instances);
int vvg_event_human(SDL_Surface** stdscr, SDL_Surface** imgscr,
                     Cap** cap_root, Player** player_root, SDL_Event* event);
int vvg_event_ai(SDL_Surface** stdscr, SDL_Surface** imgscr,
                 Cap** cap_root, Player** player_root, Player** player);

int vvg_find_caps_by_color(SDL_Surface** stdscr, SDL_Surface** imgscr,
                           Cap** cap_root, Player** player_root, Player** player,
                           int color, int xupdate);
int vvg_find_nearby_caps(Cap** cap_root, Player** player_root, Player** player);
void vvg_free_hoverlist(Cap** cap_root, Player** player);
int vvg_capture_hovercaps(Player** player);
int vvg_sanity_check(Cap** cap_root, Player** player_root, int instances);
#endif
