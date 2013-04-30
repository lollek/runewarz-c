#ifndef VVK_GAME_H
#define VVK_GAME_H

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "vvl_link.h"
#include "vvx_graphics.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TILESIZE 15

int vvg_make_map(Master* master, const char* mapname);
void vvg_free_map(Master* master);

int vvg_play_game(Master* master);
int vvg_event_human(Master* master, SDL_Event* event);
int vvg_event_human_capture(Master* master);
int vvg_event_human_mouse_new_focus(Master* master);
int vvg_event_ai(Master* master, int check_only);

int vvg_find_caps_by_color(Master* master, int color, int xupdate);
int vvg_find_nearby_caps(Master* master);
void vvg_free_hoverlist(Master* master);
int vvg_capture_hovercaps(Player** player);
int vvg_sanity_check(Master* master);
#endif
