#ifndef VVK_GAME_H
#define VVK_GAME_H

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "SDL/SDL.h"

#include "vvl_link.h"
#include "vvx_graphics.h"

int vvg_get_filesize(FILE* fp);
int vvg_load_mapfile(Master* master, const char* mapname);
int vvg_make_map(Master* master);
void vvg_free_map(Master* master);

int vvg_play_game(Master* master);
int vvg_event_human(Master* master, SDL_Event* event);
int vvg_event_human_capture(Master* master);
int vvg_event_ai(Master* master);

int vvg_find_caps_by_color(Master* master, int color, int xupdate);
int vvg_find_nearby_caps(Master* master);
void vvg_free_hoverlist(Master* master);
int vvg_capture_hovercaps(Player** player);
int vvg_sanity_check(Master* master);
#endif
