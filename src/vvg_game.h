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
int vvg_ingame_event(SDL_Surface** stdscr, SDL_Surface** imgscr,
                     Cap** cap_root, Player** player_root, SDL_Event* event);
int vvg_ingame_ai_take_turn();
void vvg_find_nearby_caps(Cap** cap_root, Player** player_root);
void vvg_free_hoverlist(Cap** cap_root, Player** player_root);
int vvg_capture_hovercaps(Cap** cap_root, Player** player_root);
int vvg_sanity_check(Cap** cap_root, Player** player_root, int instances);
#endif
