#ifndef VVK_GAME_H
#define VVK_GAME_H

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "SDL/SDL.h"

typedef struct Cap {
  
  int x;
  int y;
  int is_active;
  int color;

  struct Cap *next;
  
} Cap;

typedef struct CapList {

  int x;
  int y;

  struct CapList *next;
  
} CapList;

typedef struct Player {

  int symbol;
  int color;
  int is_player;

  struct CapList *cap_list;
  struct Player *next;
  
} Player;

int vvk_get_filesize(FILE* fp);
int vvk_load_mapfile(const char* mapname, char** buf);
int vvk_make_map(char** map_buffer, Cap** cap_root, Player** player_root);
void vvk_free_map(Cap** cap_root, Player** player_root);
int vvk_play_game(SDL_Surface** stdscr, SDL_Surface** imgscr,
                  Cap** cap_root, Player** player_root);


#endif
