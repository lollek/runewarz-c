#include "vvk_game.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TILESIZE 15

int vvk_get_filesize(FILE* fp) {
  int sz, prev;

  prev = ftell(fp);
  fseek(fp, 0L, SEEK_END);

  sz = ftell(fp);
  fseek(fp, prev, SEEK_SET);

  return sz;
}

int vvk_load_mapfile(const char* mapname, char** buf) {

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
  *buf = (char *)malloc(sizeof(char)*vvk_get_filesize(fd));
  
  fgets(minibuf, sizeof minibuf, fd);
  strcpy(*buf, minibuf);
  while ((fgets(minibuf, sizeof minibuf, fd)) != NULL)
    strcat(*buf, minibuf);

  fclose(fd);
  free(fullpath);
  
  return 0;
}

int vvk_make_map(char** map_buffer, Cap** cap_root, Player** player_root, int* instances) {

  int x, y, players = 2;
  char *p = NULL;
  Cap *cap_ptr = NULL, *cap_killer = NULL;
  Player *player_ptr = NULL, *player_killer = NULL;
  
  srand(time(0));

  /* Iterate through the map:
     # becomes a Cap (game tile) and goes into a cap_root linked list
     @ becomes a player and goes into player_root linked list
  */
  *cap_root = (Cap *)malloc(sizeof(Cap));
  cap_ptr = cap_root[0];

  *player_root = (Player *)malloc(sizeof(Player));
  player_ptr = player_root[0];

  for(p = map_buffer[0], x = 0, y = 0; *p != '\0' ; p++, x++) {
    if (*p == '\n') {
      y++;
      x = -1;
      
    } else if (*p == '#') {
      (*instances)++;
      cap_ptr->next = (Cap *)malloc(sizeof(Cap));
      cap_ptr->next->prev = cap_ptr;
      cap_ptr = cap_ptr->next;
      cap_ptr->x = x;
      cap_ptr->y = y;
      cap_ptr->color = rand() % 6 + 1; 
      cap_ptr->next = NULL;
      
    } else if (*p == '@') {
      (*instances)++;
      player_ptr->next = (Player *)malloc(sizeof(Player));
      player_ptr = player_ptr->next;
      player_ptr->symbol = players++;
      player_ptr->color = 0;

      if (players == 3)
        player_ptr->is_player = 1;
      else
        player_ptr->is_player = 0;

      player_ptr->hover_color = 0;
      player_ptr->hover_list = NULL;

      player_ptr->cap_list = (Cap *)malloc(sizeof(Cap));
      player_ptr->cap_list->x = x;
      player_ptr->cap_list->y = y;
      player_ptr->cap_list->next = NULL;
      player_ptr->cap_list->prev = NULL;

      player_ptr->next = NULL;
    }
  }

  /* Move the 2nd instance of player and cap to the first and remove the 2nd:
     This is since the 1st is unused */
  player_ptr = player_root[0];
  player_killer = player_ptr->next;
  
  player_ptr->symbol = player_killer->symbol;
  player_ptr->color = player_killer->color;
  player_ptr->is_player = player_killer->is_player;
  player_ptr->hover_color = player_killer->hover_color;
  player_ptr->hover_list = player_killer->hover_list;
  player_ptr->cap_list = player_killer->cap_list;
  player_ptr->next = player_killer->next;

  free(player_killer);

  (*cap_root)->prev = cap_ptr;
  
  cap_ptr = cap_root[0];
  cap_killer = cap_ptr->next;
  
  cap_ptr->x = cap_killer->x;
  cap_ptr->y = cap_killer->y;
  cap_ptr->color = cap_killer->color;
  cap_ptr->next = cap_killer->next;

  free(cap_killer);

  /* Free map_buffer: */
  free(*map_buffer);
  *map_buffer = NULL;

  return 0;
}

void vvk_free_map(Cap** cap_root, Player** player_root) {

  Cap *cap_ptr = NULL, *cap_killer = NULL;
  Player *player_ptr = NULL, *player_killer = NULL;
  Cap *cl_ptr = NULL, *cl_killer = NULL;

  vvk_free_hoverlist(cap_root, player_root);

  cap_ptr = cap_root[0];
  while (cap_ptr != NULL) {
    cap_killer = cap_ptr;
    cap_ptr = cap_ptr->next;
    
    free(cap_killer);
    cap_killer = NULL;
  }

  player_ptr = player_root[0];
  while (player_ptr != NULL) {
    player_killer = player_ptr;
    player_ptr = player_ptr->next;

    cl_ptr = player_killer->cap_list;
    while (cl_ptr != NULL) {
      cl_killer = cl_ptr;
      cl_ptr = cl_ptr->next;
      free(cl_killer);
      cl_killer = NULL;
    }

    free(player_killer);
    player_killer = NULL;
  }
}

int vvk_play_game(SDL_Surface** stdscr, SDL_Surface** imgscr,
                  Cap** cap_root, Player** player_root, int instances) {

  SDL_Event event;
  Player* player_ptr = NULL;

  vvk_draw_all_caps(stdscr, imgscr, cap_root, player_root);
  SDL_Flip(*stdscr);

  for (;;) {
    for (player_ptr = player_root[0]; player_ptr != NULL; player_ptr = player_ptr->next) {
      if (player_ptr->is_player)
        switch (vvk_ingame_event(stdscr, imgscr, cap_root, player_root, &event)) {
          case 1: SDL_Flip(*stdscr); break;
          case -1: return 0; break;
      }
      else
        vvk_ingame_ai_take_turn();
    }
    vvk_sanity_check(cap_root, player_root, instances);
  }
  
  return 0;
}

int vvk_ingame_event(SDL_Surface** stdscr, SDL_Surface** imgscr,
                     Cap** cap_root, Player** player_root, SDL_Event* event) {
  while (SDL_WaitEvent(event)) {
    if (event->type == SDL_KEYDOWN) {
      switch (event->key.keysym.sym) {
        case SDLK_1: {
          vvk_draw_hoverlist_blank(stdscr, imgscr, player_root);
          vvk_free_hoverlist(cap_root, player_root);

          (*player_root)->hover_color = 1;
          vvk_find_nearby_caps(cap_root, player_root);
          vvk_draw_hoverlist(stdscr, imgscr, player_root);
          return 1; break;
        }
        case SDLK_2: {
          vvk_draw_hoverlist_blank(stdscr, imgscr, player_root);
          vvk_free_hoverlist(cap_root, player_root);

          (*player_root)->hover_color = 2;
          vvk_find_nearby_caps(cap_root, player_root);
          vvk_draw_hoverlist(stdscr, imgscr, player_root);
          return 1; break;
        }
        case SDLK_3: {
          vvk_draw_hoverlist_blank(stdscr, imgscr, player_root);
          vvk_free_hoverlist(cap_root, player_root);

          (*player_root)->hover_color = 3;
          vvk_find_nearby_caps(cap_root, player_root);
          vvk_draw_hoverlist(stdscr, imgscr, player_root);
          return 1; break;
        }
        case SDLK_4: {
          vvk_draw_hoverlist_blank(stdscr, imgscr, player_root);
          vvk_free_hoverlist(cap_root, player_root);

          (*player_root)->hover_color = 4;
          vvk_find_nearby_caps(cap_root, player_root);
          vvk_draw_hoverlist(stdscr, imgscr, player_root);
          return 1; break;
        }
        case SDLK_5: {
          vvk_draw_hoverlist_blank(stdscr, imgscr, player_root);
          vvk_free_hoverlist(cap_root, player_root);

          (*player_root)->hover_color = 5;
          vvk_find_nearby_caps(cap_root, player_root);
          vvk_draw_hoverlist(stdscr, imgscr, player_root);
          return 1; break;
        }
        case SDLK_6: {
          vvk_draw_hoverlist_blank(stdscr, imgscr, player_root);
          vvk_free_hoverlist(cap_root, player_root);

          (*player_root)->hover_color = 6;
          vvk_find_nearby_caps(cap_root, player_root);
          vvk_draw_hoverlist(stdscr, imgscr, player_root);
          return 1; break;
        }
        case SDLK_RETURN: {
          vvk_capture_hovercaps(cap_root, player_root);
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

int vvk_ingame_ai_take_turn() {
  return 0;
}

void vvk_find_nearby_caps(Cap** cap_root, Player** player_root) {

  int found_caps = 0;
  Cap *cap_ptr = NULL; /* List of free caps */
  Cap *cl_ptr = NULL;  /* Iteration */
  Cap *hl_ptr = NULL;  /* List of hovercaps */

  if ((*player_root)->hover_list != NULL) {
    fprintf(stderr, "hover_list is not NULL! This should not happen\n");
    return;
  }
  
  (*player_root)->hover_list = (Cap *)malloc(sizeof(Cap));
  hl_ptr = (*player_root)->hover_list;
  hl_ptr->x = -1;
  hl_ptr->y = -1;

  cap_ptr = cap_root[0];
  while (cap_ptr != NULL) {
    if (cap_ptr->color == (*player_root)->hover_color) {
      for (cl_ptr = (*player_root)->cap_list; cl_ptr != NULL; cl_ptr = cl_ptr->next) {
        if ((cap_ptr->y == cl_ptr->y
             && (cap_ptr->x == cl_ptr->x-1 || cap_ptr->x == cl_ptr->x+1)) ||
            (cap_ptr->x == cl_ptr->x
             && (cap_ptr->y == cl_ptr->y-1 || cap_ptr->y == cl_ptr->y+1))) {

          found_caps++;
          
          hl_ptr->next = cap_ptr;

          cap_ptr->prev->next = cap_ptr->next;
          cap_ptr->next->prev = cap_ptr->prev;
          cap_ptr = cap_ptr->prev;
          
          hl_ptr->next->prev = hl_ptr;
          hl_ptr = hl_ptr->next;
          hl_ptr->next = NULL;
        }
      }
    }
    fprintf(stderr, "_");
    cap_ptr = cap_ptr->next;
  }

  if (!found_caps) {
    free(hl_ptr);
    (*player_root)->hover_list = NULL;
    return;
  }

  cap_ptr = cap_root[0];
  while (cap_ptr != NULL) {
    if (cap_ptr->color == (*player_root)->hover_color) {
      for (cl_ptr = (*player_root)->hover_list; cl_ptr != NULL; cl_ptr = cl_ptr->next) {
        if ((cap_ptr->y == cl_ptr->y
             && (cap_ptr->x == cl_ptr->x-1 || cap_ptr->x == cl_ptr->x+1))
            ||
            (cap_ptr->x == cl_ptr->x
             && (cap_ptr->y == cl_ptr->y-1 || cap_ptr->y == cl_ptr->y+1))) {

          found_caps++;

          hl_ptr->next = cap_ptr;

          cap_ptr->prev->next = cap_ptr->next;
          cap_ptr->next->prev = cap_ptr->prev;
          cap_ptr = cap_ptr->prev;
          
          hl_ptr->next->prev = hl_ptr;
          hl_ptr = hl_ptr->next;
          hl_ptr->next = NULL;
        }
      }
    }
    fprintf(stderr, "-");
    cap_ptr = cap_ptr->next;
  }

  (*player_root)->hover_list->prev = hl_ptr;
  
  hl_ptr = (*player_root)->hover_list;
  cl_ptr = hl_ptr->next;
  hl_ptr->x = cl_ptr->x;
  hl_ptr->y = cl_ptr->y;
  hl_ptr->next = cl_ptr->next;
  free(cl_ptr);
}

void vvk_free_hoverlist(Cap** cap_root, Player** player_root) {

  Cap *cl_ptr = NULL;
  Cap *hl_ptr = NULL;

  if ((*player_root)->hover_list == NULL)
    return;

  for (cl_ptr = cap_root[0]; cl_ptr->next != NULL; cl_ptr = cl_ptr->next);
  hl_ptr = (*player_root)->hover_list;
  
  cl_ptr->next = hl_ptr;
  hl_ptr->prev = cl_ptr;
  (*player_root)->hover_list = NULL;
}
  
int vvk_capture_hovercaps(Cap** cap_root, Player** player_root) {

  Cap *cl_ptr = NULL;
  Cap *hl_ptr = NULL;

  if ((*player_root)->hover_list == NULL)
    return 1;

  for (cl_ptr = (*player_root)->cap_list; cl_ptr->next != NULL; cl_ptr = cl_ptr->next);
  
  hl_ptr = (*player_root)->hover_list;
  cl_ptr->next = hl_ptr;
  hl_ptr->prev = cl_ptr;
  (*player_root)->hover_list = NULL;

  (*player_root)->color = (*player_root)->hover_color;

  printf("CapList:\n");
  for (cl_ptr = (*player_root)->cap_list; cl_ptr != NULL; cl_ptr = cl_ptr->next) {
    printf("%d : %d\t", cl_ptr->x, cl_ptr->y);
  }
  printf("\n");
  return 0;
}

int vvk_sanity_check(Cap** cap_root, Player** player_root, int instances) {

  int curr_cap = 0;
  int curr_cl = 0;
  int curr_hl = 0;
  int curr_inst = 0;
  Cap *cap_ptr = NULL;
  Player *player_ptr = NULL;

  for (cap_ptr = cap_root[0]; cap_ptr != NULL; cap_ptr = cap_ptr->next)
    curr_cap++;
  for (player_ptr = player_root[0]; player_ptr != NULL; player_ptr = player_ptr->next)
    for (cap_ptr = player_ptr->cap_list; cap_ptr != NULL; cap_ptr = cap_ptr->next)
      curr_cl++;
  for (cap_ptr = (*player_root)->hover_list; cap_ptr != NULL; cap_ptr = cap_ptr->next)
    curr_hl++;

  curr_inst = curr_cap + curr_cl + curr_hl;

  if (!(curr_inst == instances)) {
    fprintf(stderr, "Missing instances! Was %d, is %d (%d-%d-%d)\n",
            instances, curr_inst, curr_cap, curr_cl, curr_hl);

    for (player_ptr = player_root[0]; player_ptr != NULL; player_ptr = player_ptr->next) {
      fprintf(stderr, "Player!\n");
      for (cap_ptr = player_ptr->cap_list; cap_ptr != NULL; cap_ptr = cap_ptr->next) {
        fprintf(stderr, "\t%d-%d", cap_ptr->x, cap_ptr->y);
      }
      fprintf(stderr, "\n");
    }
    return 1;
  }
  return 0;
}
