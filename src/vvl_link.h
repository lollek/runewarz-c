#ifndef VVK_LINK_H
#define VVK_LINK_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Cap {
  
  int x;
  int y;
  int color;

  struct Cap *next;
  struct Cap *prev;
  
} Cap;

typedef struct Player {

  int symbol;
  int color;
  int is_player;
  int hover_color;

  struct Cap *hover_list;
  struct Cap *cap_list;
  struct Player *next;
  
} Player;

int vvl_cap_init(Cap** cap_node);
void vvl_cap_exit(Cap** cap_node);
void vvl_cap_add(Cap** cap_node, int x, int y, int color);
int vvl_cap_move_all(Cap** node_from, Cap** node_to);
int vvl_cap_move(Cap** node_from, Cap** node_to, Cap* cap);

int vvl_player_init(Player** player_root);
void vvl_player_exit(Player** player_root);
void vvl_player_add(Player** player_root, int symbol, int x, int y);

#endif
