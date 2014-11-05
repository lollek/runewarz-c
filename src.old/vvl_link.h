#ifndef VVK_LINK_H
#define VVK_LINK_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

typedef struct Map {

  char *name;
  struct Map *next;

} Map;

typedef struct Cap {
  
  char x;
  char y;
  char color;

  struct Cap *next;
  struct Cap *prev;
  
} Cap;

typedef struct Player {

  char symbol;
  char color;
  char is_player;
  char hover_color;

  struct Cap *hover_list;
  struct Cap *cap_list;
  struct Player *next;
  
} Player;

typedef struct Master {

  SDL_Surface *stdscr;
  SDL_Surface *imgscr;

  Player *player_root;
  Player *current_player;

  Cap *cap_root;

  TTF_Font *font;

  char players;
  char colors;
  short instances;

  char map_width;
  char map_height;
  short map_offset_x;
  short map_offset_y;
  short mousecap_x;
  short mousecap_y;
  
} Master;

int vvl_map_add(Map* map_root);
void vvl_map_sort(Map *map_root);
void vvl_map_remove(Map* map_root);

int vvl_cap_init(Cap** cap_node);
void vvl_cap_exit(Cap** cap_node);
void vvl_cap_add(Cap** cap_node, int x, int y, int color);
int vvl_cap_move_all(Cap** node_from, Cap** node_to);
int vvl_cap_move(Cap** node_from, Cap** node_to, Cap* cap);

int vvl_player_init(Player** player_root);
void vvl_player_exit(Player** player_root);
void vvl_player_add(Player** player_root, int symbol, int x, int y);

#endif
