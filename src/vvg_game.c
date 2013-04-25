#include "vvg_game.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TILESIZE 15

int vvg_get_filesize(FILE* fp) {
  int sz, prev;

  prev = ftell(fp);
  fseek(fp, 0L, SEEK_END);

  sz = ftell(fp);
  fseek(fp, prev, SEEK_SET);

  return sz;
}

int vvg_load_mapfile(const char* mapname, char** buf) {

  FILE *fd = NULL;
  const char *prepath = "maps/";
  char *fullpath = malloc(strlen(mapname) + strlen(prepath) + 1);
  char minibuf[256];

  /* Combine mapname + prepath into a path: */
  strcpy(fullpath, prepath);
  strcat(fullpath, mapname);

  /* Open the file at location: */
  fd = fopen(fullpath, "r");
  if (fd == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", fullpath);
    return 1;
  }

  /* Fetch the file into buf: */
  *buf = (char *)malloc(sizeof(char)*vvg_get_filesize(fd));
  
  fgets(minibuf, sizeof minibuf, fd);
  strcpy(*buf, minibuf);
  while ((fgets(minibuf, sizeof minibuf, fd)) != NULL)
    strcat(*buf, minibuf);

  fclose(fd);
  free(fullpath);
  
  return 0;
}

int vvg_make_map(char** map_buffer, Cap** cap_root, Player** player_root, int* instances) {

  int x, y, players = 0;
  char *p = NULL;

  /* Start srand for randomizer */
  srand(time(0));

  /* Create a node to serve as root for all free caps: */
  if (vvl_cap_init(cap_root) == 1) return 1;

  /* Create a node to server as root for all players: */
  if (vvl_player_init(player_root) == 1) return 1;
  
  /* Iterate through the map:
     # becomes a Cap (game tile) and goes into a cap_root linked list
     @ becomes a player and goes into player_root linked list */

  for(p = map_buffer[0], x = 0, y = 0; *p != '\0' ; p++, x++) {
    if (*p == '\n') {
      y++;
      x = -1;
      
    } else if (*p == '#') {
      (*instances)++;
      vvl_cap_add(cap_root, x, y, rand() % 6 + 1);
      
    } else if (*p == '@') {
      (*instances)++;
      vvl_player_add(player_root, players++, x, y);
    }
  }

  /* Free map_buffer: */
  free(*map_buffer);
  *map_buffer = NULL;

  return 0;
}

void vvg_free_map(Cap** cap_root, Player** player_root) {

  vvl_cap_exit(cap_root);
  vvl_player_exit(player_root);

}

int vvg_play_game(SDL_Surface** stdscr, SDL_Surface** imgscr,
                  Cap** cap_root, Player** player_root, int instances) {

  SDL_Event event;
  Player* pl_ptr = NULL;

  vvk_draw_all_caps(stdscr, imgscr, cap_root, player_root);
  SDL_Flip(*stdscr);

  for (;;) {
    for (pl_ptr = (*player_root)->next; pl_ptr != NULL; pl_ptr = pl_ptr->next) {
      if (pl_ptr->is_player)
        switch (vvg_ingame_event(stdscr, imgscr, cap_root, player_root, &event)) {
          case 1: SDL_Flip(*stdscr); break;
          case -1: return 0; break;
      }
      else
        vvg_ingame_ai_take_turn();
    }
    vvg_sanity_check(cap_root, player_root, instances);
  }
  
  return 0;
}

int vvg_ingame_event(SDL_Surface** stdscr, SDL_Surface** imgscr,
                     Cap** cap_root, Player** player_root, SDL_Event* event) {

  Player *pl_ptr = (*player_root)->next;
  
  while (SDL_WaitEvent(event)) {
    if (event->type == SDL_KEYDOWN) {
      switch (event->key.keysym.sym) {
        case SDLK_1: {
          vvk_draw_hoverlist_blank(stdscr, imgscr, player_root);

          pl_ptr->hover_color = 1;
          vvg_find_nearby_caps(cap_root, player_root);
          vvk_draw_hoverlist(stdscr, imgscr, player_root);
          return 1; break;
        }
        case SDLK_2: {
          vvk_draw_hoverlist_blank(stdscr, imgscr, player_root);

          pl_ptr->hover_color = 2;
          vvg_find_nearby_caps(cap_root, player_root);
          vvk_draw_hoverlist(stdscr, imgscr, player_root);
          return 1; break;
        }
        case SDLK_3: {
          vvk_draw_hoverlist_blank(stdscr, imgscr, player_root);

          pl_ptr->hover_color = 3;
          vvg_find_nearby_caps(cap_root, player_root);
          vvk_draw_hoverlist(stdscr, imgscr, player_root);
          return 1; break;
        }
        case SDLK_4: {
          vvk_draw_hoverlist_blank(stdscr, imgscr, player_root);

          pl_ptr->hover_color = 4;
          vvg_find_nearby_caps(cap_root, player_root);
          vvk_draw_hoverlist(stdscr, imgscr, player_root);
          return 1; break;
        }
        case SDLK_5: {
          vvk_draw_hoverlist_blank(stdscr, imgscr, player_root);

          pl_ptr->hover_color = 5;
          vvg_find_nearby_caps(cap_root, player_root);
          vvk_draw_hoverlist(stdscr, imgscr, player_root);
          return 1; break;
        }
        case SDLK_6: {
          vvk_draw_hoverlist_blank(stdscr, imgscr, player_root);

          pl_ptr->hover_color = 6;
          vvg_find_nearby_caps(cap_root, player_root);
          vvk_draw_hoverlist(stdscr, imgscr, player_root);
          return 1; break;
        }
        case SDLK_0: {
          vvk_draw_hoverlist_blank(stdscr, imgscr, player_root);
          vvg_free_hoverlist(cap_root, player_root);
          return 1; break;
        }
        case SDLK_RETURN: {
          vvg_capture_hovercaps(cap_root, player_root);
          vvk_draw_capture(stdscr, imgscr, player_root);
          printf("done!\n");
          return 1; break;
        }
        case SDLK_ESCAPE: return -1; break;
        default: return 0; break;
      }
    }
    else if (event->type == SDL_MOUSEMOTION) {}
    else if (event->type == SDL_MOUSEBUTTONDOWN) {}
    else return 0;
  }
  return 0;
}

int vvg_ingame_ai_take_turn() {
  return 0;
}

void vvg_find_nearby_caps(Cap** cap_root, Player** player_root) {

  Player *pl_ptr = (*player_root)->next;
  
  int found_caps = 0;
  Cap *cap_ptr = NULL; /* List of free caps */
  Cap *cl_ptr = NULL;  /* Iteration */
  Cap *hl_ptr = NULL;  /* List of hovercaps */

  vvg_free_hoverlist(cap_root, player_root);

  /* Find caps that are close to player's caps: */
  for (cap_ptr = (*cap_root)->next; cap_ptr != NULL; cap_ptr = cap_ptr->next) {
    if (cap_ptr->color == pl_ptr->hover_color) {
      for (cl_ptr = pl_ptr->cap_list->next; cl_ptr != NULL; cl_ptr = cl_ptr->next) {
        if ((cap_ptr->y == cl_ptr->y
             && (cap_ptr->x == cl_ptr->x-1 || cap_ptr->x == cl_ptr->x+1)) ||
            (cap_ptr->x == cl_ptr->x
             && (cap_ptr->y == cl_ptr->y-1 || cap_ptr->y == cl_ptr->y+1))) {

          found_caps++;
          
          if (cap_ptr != cap_root[0]) hl_ptr = cap_ptr->prev;
          else hl_ptr = cap_root[0];
          vvl_cap_move(cap_root, &pl_ptr->hover_list, cap_ptr);
          cap_ptr = hl_ptr;
        }
      }
    }
  }

  if (!found_caps) {
    printf("No caps found..\n");
    return;
  }

  /* If there are any, find caps that are close to the found caps: */
  for (cap_ptr = (*cap_root)->next; cap_ptr != NULL; cap_ptr = cap_ptr->next) {
    if (cap_ptr->color == pl_ptr->hover_color) {
      for (cl_ptr = pl_ptr->hover_list->next; cl_ptr != NULL; cl_ptr = cl_ptr->next) {
        if ((cap_ptr->y == cl_ptr->y
             && (cap_ptr->x == cl_ptr->x-1 || cap_ptr->x == cl_ptr->x+1)) ||
            (cap_ptr->x == cl_ptr->x
             && (cap_ptr->y == cl_ptr->y-1 || cap_ptr->y == cl_ptr->y+1))) {

          found_caps++;
          
          if (cap_ptr != cap_root[0]) hl_ptr = cap_ptr->prev;
          else hl_ptr = cap_root[0];
          vvl_cap_move(cap_root, &pl_ptr->hover_list, cap_ptr);
          cap_ptr = hl_ptr;
        }
      }
    }
  }

  printf("%d caps found\n", found_caps);

  /* Bug check: check if any in the hoverlist are the wrong color: */
  for (cap_ptr = pl_ptr->hover_list->next; cap_ptr != NULL; cap_ptr = cap_ptr->next)
    if (cap_ptr->color != pl_ptr->hover_color) {
      fprintf(stderr, "%d,%d cap is the wrong color\n", cap_ptr->x, cap_ptr->y);
    }
  
  /*
  hl_ptr = pl_ptr->hover_list;
  cap_ptr = (*cap_root)->next;
  while (cap_ptr != NULL) {
    if (cap_ptr->color == pl_ptr->hover_color) {
      for (cl_ptr = pl_ptr->cap_list->next; cl_ptr != NULL; cl_ptr = cl_ptr->next) {
        if ((cap_ptr->y == cl_ptr->y
             && (cap_ptr->x == cl_ptr->x-1 || cap_ptr->x == cl_ptr->x+1)) ||
            (cap_ptr->x == cl_ptr->x
             && (cap_ptr->y == cl_ptr->y-1 || cap_ptr->y == cl_ptr->y+1))) {

          found_caps++;

          hl_ptr->next = cap_ptr;
          if (cap_ptr->prev->next != NULL)
            cap_ptr->prev->next = cap_ptr->next;
          if (cap_ptr->next != NULL)
            cap_ptr->next->prev = cap_ptr->prev;

          if (cap_ptr->prev->next != NULL)
            cap_ptr = cap_ptr->prev;
          else
            cap_ptr = cap_root[0];
          
          hl_ptr->next->prev = hl_ptr;
          hl_ptr->next->next = NULL;
          hl_ptr = hl_ptr->next;
        }
      }
    }
    fprintf(stderr, "_");
    cap_ptr = cap_ptr->next;
    } 


  cap_ptr = (*cap_root)->next;
  while (cap_ptr != NULL) {
    if (cap_ptr->color == pl_ptr->hover_color) {
      for (cl_ptr = pl_ptr->hover_list->next; cl_ptr != NULL; cl_ptr = cl_ptr->next) {
        if ((cap_ptr->y == cl_ptr->y
             && (cap_ptr->x == cl_ptr->x-1 || cap_ptr->x == cl_ptr->x+1))
            ||
            (cap_ptr->x == cl_ptr->x
             && (cap_ptr->y == cl_ptr->y-1 || cap_ptr->y == cl_ptr->y+1))) {

          found_caps++;

          hl_ptr->next = cap_ptr;

          if (cap_ptr->prev->next != NULL)
            cap_ptr->prev->next = cap_ptr->next;
          if (cap_ptr->next != NULL)
            cap_ptr->next->prev = cap_ptr->prev;

          if (cap_ptr->prev->next != NULL)
            cap_ptr = cap_ptr->prev;
          else
            cap_ptr = cap_root[0];
          
          hl_ptr->next->prev = hl_ptr;
          hl_ptr->next->next = NULL;
          hl_ptr = hl_ptr->next;
        }
      }
    }
    fprintf(stderr, "-");
    cap_ptr = cap_ptr->next;
  }
  
  pl_ptr->hover_list->next->prev = hl_ptr;
  */
}

void vvg_free_hoverlist(Cap** cap_root, Player** player_root) {

  Player *pl_ptr = (*player_root)->next;
  Cap *cl_ptr = NULL;
  Cap *hl_ptr = NULL;

  if (pl_ptr->hover_list->next == NULL)
    return;

  for (cl_ptr = (*cap_root)->next; cl_ptr->next != NULL; cl_ptr = cl_ptr->next);
  hl_ptr = pl_ptr->hover_list->next;
  
  cl_ptr->next = hl_ptr;
  hl_ptr->prev = cl_ptr;
  pl_ptr->hover_list->next = NULL;

  while (cl_ptr->next != NULL) cl_ptr = cl_ptr->next;
  (*cap_root)->next->prev = cl_ptr;
  
}
  
int vvg_capture_hovercaps(Cap** cap_root, Player** player_root) {

  Player *pl_ptr = (*player_root)->next;
  Cap *cl_ptr = NULL;
  Cap *hl_ptr = NULL;

  if (pl_ptr->hover_list->next == NULL)
    return 1;

  for (cl_ptr = pl_ptr->cap_list->next; cl_ptr->next != NULL; cl_ptr = cl_ptr->next);
  
  hl_ptr = pl_ptr->hover_list->next;
  cl_ptr->next = hl_ptr;
  hl_ptr->prev = cl_ptr;

  while (cl_ptr->next != NULL) cl_ptr = cl_ptr->next;
  pl_ptr->cap_list->next->prev = cl_ptr;

  pl_ptr->color = pl_ptr->hover_color;

  pl_ptr->hover_list->next = NULL;

  return 0;
}

int vvg_sanity_check(Cap** cap_root, Player** player_root, int instances) {

  int curr_cap = 0;
  int curr_cl = 0;
  int curr_hl = 0;
  int curr_inst = 0;
  Cap *cap_ptr = NULL;
  Player *pl_ptr = NULL;

  for (cap_ptr = (*cap_root)->next; cap_ptr != NULL; cap_ptr = cap_ptr->next)
    curr_cap++;
  for (pl_ptr = (*player_root)->next; pl_ptr != NULL; pl_ptr = pl_ptr->next) {
    for (cap_ptr = pl_ptr->cap_list->next; cap_ptr != NULL; cap_ptr = cap_ptr->next)
      curr_cl++;
    for (cap_ptr = pl_ptr->hover_list->next; cap_ptr != NULL; cap_ptr = cap_ptr->next)
      curr_hl++;
  }

  curr_inst = curr_cap + curr_cl + curr_hl;

  if (!(curr_inst == instances)) {
    fprintf(stderr, "Missing instances! Was %d, is %d (%d-%d-%d)\n",
            instances, curr_inst, curr_cap, curr_cl, curr_hl);

    for (pl_ptr = (*player_root)->next; pl_ptr != NULL; pl_ptr = pl_ptr->next) {
      fprintf(stderr, "Player!\n");
      for (cap_ptr = pl_ptr->cap_list->next; cap_ptr != NULL; cap_ptr = cap_ptr->next) {
        fprintf(stderr, "\t%d-%d", cap_ptr->x, cap_ptr->y);
      }
      fprintf(stderr, "\n");
    }
    return 1;
  }
  return 0;
}
