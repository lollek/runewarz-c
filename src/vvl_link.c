#include "vvl_link.h"

/* Create a linked cap-list from pointer: */
int vvl_cap_init(Cap** cap_node) {

  Cap *cap_p = NULL;
  
  if (*cap_node != NULL) {
    fprintf(stderr, "vvl_cap_init: cap_node already initialized!\n");
    return 1;
  }
  
  *cap_node = (Cap *)malloc(sizeof(Cap));
  cap_p = cap_node[0];
  cap_p->x = -1;
  cap_p->y = -1;
  cap_p->color = -1;
  cap_p->next = NULL;
  cap_p->prev = NULL;
  
  return 0;
}

/* Destroy a linked cap-list from pointer: */
void vvl_cap_exit(Cap** cap_node) {

  Cap *cap_p = cap_node[0];
  Cap *cap_p2 = NULL;

  while (cap_p != NULL) {
    cap_p2 = cap_p;
    cap_p = cap_p->next;

    free(cap_p2);
  }
}

/* Create a new cap to a cap-list: */
void vvl_cap_add(Cap** cap_node, int x, int y, int color) {

  Cap *cap_p = NULL;
  for (cap_p = cap_node[0]; cap_p->next != NULL; cap_p = cap_p->next);

  cap_p->next = (Cap *)malloc(sizeof(Cap));

  /* cap_node should never be linked to: */
  if (cap_p != cap_node[0]) {
    cap_p->next->prev = cap_p;
    (*cap_node)->next->prev = cap_p->next;
  }
  else cap_p->next->prev = cap_p->next;

  cap_p = cap_p->next;
  cap_p->x = x;
  cap_p->y = y;
  cap_p->color = color;;
  cap_p->next = NULL;
}

int vvl_cap_move_all(Cap** node_from, Cap** node_to) {

  Cap *cap_fp = NULL;
  Cap *cap_tp = NULL;
  
  if (*node_from == NULL) {
    fprintf(stderr, "vvl_cap_move_all: node_from is NULL!\n");
    return 1;
  } else if ((*node_from)->next == NULL) {
    fprintf(stderr, "vvl_cap_move_all: refusing to move an empty list!\n");
    return 1;
  } else if (*node_to == NULL) {
    fprintf(stderr, "vvl_cap_move_all: node_to is NULL!\n");
    return 1;
  } 

  for (cap_tp = node_to[0]; cap_tp->next != NULL; cap_tp = cap_tp->next);
  cap_fp = (*node_from)->next;

  cap_tp->next = cap_fp;
  if (cap_tp != node_to[0])
    cap_fp->prev = cap_tp;
  (*node_from)->next = NULL;

  while (cap_tp->next != NULL) cap_tp = cap_tp->next;
  (*node_to)->next->prev = cap_tp;

  return 0;
}

int vvl_cap_move(Cap** node_from, Cap** node_to, Cap* cap) {

  Cap *cap_fp = NULL;
  Cap *cap_tp = NULL;
  
  if (*node_from == NULL) {
    fprintf(stderr, "vvl_cap_move: node_from is NULL!\n");
    return 1;
  } else if (*node_to == NULL) {
    fprintf(stderr, "vvl_cap_move: node_to is NULL!\n");
    return 1;
  } else if (cap == NULL) {
    fprintf(stderr, "vvl_cap_move: cap is NULL!\n");
    return 1;
  }

  for (cap_fp = node_from[0]; cap_fp != cap && cap_fp != NULL; cap_fp = cap_fp->next);
  if (cap_fp == NULL) {
    fprintf(stderr, "vvl_cap_move: cap not found in node_from!\n");
    return 1;
  }

  for (cap_tp = node_to[0]; cap_tp->next != NULL; cap_tp = cap_tp->next);

  /* Find: */
  cap_tp->next = cap;

  /* Remove old link: */
  if (cap->prev->next != NULL)
    cap->prev->next = cap->next;

  if (cap->next != NULL) {
    cap->next->prev = cap->prev;
    if (cap == (*node_from)->next)
      (*node_from)->next = cap->next;

  } else {
    if (cap == (*node_from)->next)
      (*node_from)->next = NULL;
    else
      (*node_from)->next->prev = cap->prev;
  }


  /* Add new link: */
  cap->next = NULL;
  if (cap_tp != node_to[0]) {
    cap->prev = cap_tp;
    (*node_to)->next->prev = cap;
  } else cap->prev = cap;

  return 0;
}

/* Create a linked player-list from pointer: */
int vvl_player_init(Player** player_root) {

  Player *pl_p = NULL;

  if (*player_root != NULL) {
    fprintf(stderr, "vvl_player_init: player_root already initialized!\n");
    return 1;
  }

  *player_root = (Player *)malloc(sizeof(Player));
  pl_p = player_root[0];
  pl_p->symbol = -1;
  pl_p->color = -1;
  pl_p->is_player = -1;
  pl_p->hover_color = -1;
  pl_p->hover_list = NULL;
  pl_p->cap_list = NULL;
  pl_p->next = NULL;

  return 0;
}

/* Destroy a linked player-list from pointer: */
void vvl_player_exit(Player** player_root) {

  Player *pl_p = player_root[0];
  Player *pl_p2 = NULL;

  while (pl_p != NULL) {
    pl_p2 = pl_p;
    pl_p = pl_p->next;

    vvl_cap_exit(&pl_p2->hover_list);
    vvl_cap_exit(&pl_p2->cap_list);
    free(pl_p2);
  }
}

/* Create a new player to a player-list: */
void vvl_player_add(Player** player_root, int symbol, int x, int y) {

  Player *pl_p = NULL;
  for (pl_p = player_root[0]; pl_p->next != NULL; pl_p = pl_p->next);

  pl_p->next = (Player *)malloc(sizeof(Player));
  pl_p = pl_p->next;
  pl_p->symbol = symbol;
  pl_p->color = 0;
  pl_p->is_player = (symbol == 1) ? 1 : 0;
  pl_p->hover_color = 0;

  pl_p->hover_list = NULL;
  vvl_cap_init(&pl_p->hover_list);
  pl_p->cap_list = NULL;
  vvl_cap_init(&pl_p->cap_list);

  vvl_cap_add(&pl_p->cap_list, x, y, 0);

  pl_p->next = NULL;

}

