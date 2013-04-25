#include "vvg_game.h"

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
      vvl_player_add(player_root, ++players, x, y);
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
  int status = 0;
  SDL_Event event;
  Player* curr_pl = NULL;

  vvx_draw_all_caps(stdscr, imgscr, cap_root, player_root);
  SDL_Flip(*stdscr);

  for (;;) {
    for (curr_pl = (*player_root)->next; curr_pl != NULL; curr_pl = curr_pl->next) {
      if (curr_pl->is_player)
        do
          switch (status = vvg_event_human(stdscr, imgscr, cap_root, player_root, &event)) {
            case 2: /* Fall through */
            case 1: SDL_Flip(*stdscr); break;
            case -1: return 0; break;
          }
        while (status != 2);
      else {
        vvg_event_ai(stdscr, imgscr, cap_root, player_root, &curr_pl);
        SDL_Flip(*stdscr);
      }
    }
    vvg_sanity_check(cap_root, player_root, instances);
  }
  
  return 0;
}

int vvg_event_human(SDL_Surface** stdscr, SDL_Surface** imgscr,
                    Cap** cap_root, Player** player_root, SDL_Event* event) {

  Player *pl_ptr = (*player_root)->next;
  Player *pl_aux = NULL;
  
  while (SDL_WaitEvent(event)) {
    if (event->type == SDL_KEYDOWN) {
      switch (event->key.keysym.sym) {
        case SDLK_1: {
          vvg_find_caps_by_color(stdscr, imgscr, cap_root, player_root, &pl_ptr, 1, 1);
          return 1; break;
        }
        case SDLK_2: {
          vvg_find_caps_by_color(stdscr, imgscr, cap_root, player_root, &pl_ptr, 2, 1);
          return 1; break;
        }
        case SDLK_3: {
          vvg_find_caps_by_color(stdscr, imgscr, cap_root, player_root, &pl_ptr, 3, 1);
          return 1; break;
        }
        case SDLK_4: {
          vvg_find_caps_by_color(stdscr, imgscr, cap_root, player_root, &pl_ptr, 4, 1);
          return 1; break;
        }
        case SDLK_5: {
          vvg_find_caps_by_color(stdscr, imgscr, cap_root, player_root, &pl_ptr, 5, 1);
          return 1; break;
        }
        case SDLK_6: {
          vvg_find_caps_by_color(stdscr, imgscr, cap_root, player_root, &pl_ptr, 6, 1);
          return 1; break;
        }
        case SDLK_0: {
          vvx_hide_hoverlist(stdscr, imgscr, &pl_ptr);
          vvg_free_hoverlist(cap_root, &pl_ptr);
          return 1; break;
        }
        case SDLK_RETURN: {

          for (pl_aux = pl_ptr->next; pl_aux != NULL; pl_aux = pl_aux->next)
            if (pl_ptr->hover_color == pl_aux->color) {
              return 0; break;
            }
          if (vvg_capture_hovercaps(&pl_ptr) == 1) {
            return 0; break;
          }
          vvx_draw_capture(stdscr, imgscr, &pl_ptr);
          return 2; break;
        }
        case SDLK_ESCAPE: return -1; break;
        default: continue; break;
      }
    }
    else if (event->type == SDL_MOUSEMOTION) {}
    else if (event->type == SDL_MOUSEBUTTONDOWN) {}
    else return 0;
  }
  return 0;
}

int vvg_event_ai(SDL_Surface** stdscr, SDL_Surface** imgscr,
                 Cap** cap_root, Player** player_root, Player** player) {

  int top_c, top_s = 0;
  int curr_c, curr_s;
  
  for (curr_c = 1; curr_c <= 6; curr_c++) {
    curr_s = vvg_find_caps_by_color(stdscr, imgscr, cap_root, player_root, player, curr_c, 0);
    if (curr_s > top_s) {
      top_s = curr_s;
      top_c = curr_c;
    }
  }
  
  if (top_s == 0) {
    printf("I'm stuck :( //AI\n");
    return 0;
  }

  vvg_find_caps_by_color(stdscr, imgscr, cap_root, player_root, player, top_c, 0);
  vvg_capture_hovercaps(player);
  vvx_draw_capture(stdscr, imgscr, player);
  
  return 0;
}

int vvg_find_caps_by_color(SDL_Surface** stdscr, SDL_Surface** imgscr,
                           Cap** cap_root, Player** player_root, Player** player,
                           int color, int xupdate) {

  int found_caps;
  
  if (xupdate)
    vvx_hide_hoverlist(stdscr, imgscr, player);

  (*player)->hover_color = color;
  found_caps = vvg_find_nearby_caps(cap_root, player_root, player);
  if (xupdate)
    vvx_draw_hoverlist(stdscr, imgscr, player, found_caps);

  if (found_caps < 0) {
    fprintf(stderr, "vvg_find_caps_by_color: vvg_find_nearby_caps return -1!\n");
    return 0;
  }
  return found_caps;
}

int vvg_find_nearby_caps(Cap** cap_root, Player** player_root, Player** player) {

  Player *pl_ptr = NULL;
  
  int found_caps = 0;
  int curr_caps;
  int is_colorblocked = 0;
  Cap *cap_ptr = NULL; /* List of free caps */
  Cap *cl_ptr = NULL;  /* Iteration */
  Cap *hl_ptr = NULL;  /* List of hovercaps */

  for (pl_ptr = (*player_root)->next; pl_ptr != NULL; pl_ptr = pl_ptr->next) {
    if (pl_ptr->color == (*player)->hover_color && pl_ptr != (*player)) {
      is_colorblocked = 1;
      break;
    }
  }
  
  pl_ptr = (*player);

  vvg_free_hoverlist(cap_root, &pl_ptr);

  /* Find caps that are close to player's caps: */
  for (cap_ptr = (*cap_root)->next; cap_ptr != NULL; cap_ptr = cap_ptr->next) {
    if (cap_ptr->color == pl_ptr->hover_color) {
      
      curr_caps = found_caps;
      cl_ptr = pl_ptr->cap_list->next;

      while (cl_ptr != NULL && curr_caps == found_caps) {
        if ((cap_ptr->y == cl_ptr->y
             && (cap_ptr->x == cl_ptr->x-1 || cap_ptr->x == cl_ptr->x+1)) ||
            (cap_ptr->x == cl_ptr->x
             && (cap_ptr->y == cl_ptr->y-1 || cap_ptr->y == cl_ptr->y+1))) {

          found_caps++;

          if (cap_ptr != (*cap_root)->next) hl_ptr = cap_ptr->prev;
          else hl_ptr = cap_root[0];
          
          if (vvl_cap_move(cap_root, &pl_ptr->hover_list, cap_ptr) == 1)
            return -1;
          cap_ptr = hl_ptr;
          
        }
        cl_ptr = cl_ptr->next;
      }
    }
  }

  if (!found_caps)
    return 0;

  /* If there are any, find caps that are close to the found caps: */
  for (cap_ptr = (*cap_root)->next; cap_ptr != NULL; cap_ptr = cap_ptr->next) {
    if (cap_ptr->color == pl_ptr->hover_color) {

      curr_caps = found_caps;
      cl_ptr = pl_ptr->hover_list->next;

      while (cl_ptr != NULL && curr_caps == found_caps) {
        if ((cap_ptr->y == cl_ptr->y
             && (cap_ptr->x == cl_ptr->x-1 || cap_ptr->x == cl_ptr->x+1)) ||
            (cap_ptr->x == cl_ptr->x
             && (cap_ptr->y == cl_ptr->y-1 || cap_ptr->y == cl_ptr->y+1))) {

          found_caps++;
          
          if (cap_ptr != (*cap_root)->next) hl_ptr = cap_ptr->prev;
          else hl_ptr = cap_root[0];
          
          if (vvl_cap_move(cap_root, &pl_ptr->hover_list, cap_ptr) == 1)
            return -1;
          cap_ptr = (*cap_root)->next;

        }
        cl_ptr = cl_ptr->next;
      }
    }
  }

  if (!is_colorblocked)
    return found_caps;
  else
    return 0;
}

void vvg_free_hoverlist(Cap** cap_root, Player** player) {

  if ((*player)->hover_list->next == NULL)
    return;

  vvl_cap_move_all(&(*player)->hover_list, cap_root);
}
  
int vvg_capture_hovercaps(Player** player) {

  if ((*player)->hover_list->next == NULL)
    return 1;

  vvl_cap_move_all(&(*player)->hover_list, &(*player)->cap_list);
  (*player)->color = (*player)->hover_color;
  (*player)->hover_color = 0;

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
