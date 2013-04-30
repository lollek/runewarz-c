#include "vvg_game.h"

/* Create a map from file */
int vvg_make_map(Master* master, const char* mapname) {

  FILE *fd = NULL;
  const char *prepath = "maps/";
  char *fullpath = (char *)malloc(strlen(mapname) + strlen(prepath) + 1);
  char x = 0, y = 0;
  int p;

  /* Open mapfile: */
  strcpy(fullpath, prepath);
  strcat(fullpath, mapname);
  fd = fopen(fullpath, "r");
  if (fd == NULL) {
    fprintf(stderr, "Failed to open file: %s\n", fullpath);
    free(fullpath);
    return 1;
  }
  
  master->instances = 0;
  master->players = 0;
  master->map_width = 0;
  master->map_height = 0;

  /* Start srand for randomizer */
  srand(time(0));

  /* Create a node to serve as root for all free caps: */
  if (vvl_cap_init(&master->cap_root) == 1) return 1;

  /* Create a node to server as root for all players: */
  if (vvl_player_init(&master->player_root) == 1) return 1;
  
  /* Iterate through the map:
     # becomes a Cap (game tile) and goes into a cap_root linked list
     @ becomes a player and goes into player_root linked list */

  while ((p = fgetc(fd)) != EOF) {
    if (p == '\n') {
      y++;
      x = -1;
      
    } else if (p == '#') {
      master->instances++;
      vvl_cap_add(&master->cap_root, x, y, rand() % 6 + 1);

      if (x > master->map_width) master->map_width = x;
      if (y > master->map_height) master->map_height = y;
      
    } else if (p == '@') {
      master->instances++;
      vvl_player_add(&master->player_root, ++master->players, x, y);

      if (x > master->map_width) master->map_width = x;
      if (y > master->map_height) master->map_height = y;
    }
    x++;
  }

  fclose(fd);
  free(fullpath);

  if (master->map_width >= 50 || master->map_height >= 30) {
    fprintf(stderr, "Map is too big! \n\
Please don't make it wider than 50 runes in a row or higher than 30\n");
    return 2;
  } else if (master->players > 4) {
    fprintf(stderr, "More than 4 players is not allowed on a map\n");
    return 2;
  }

  master->map_offset_x = (SCREEN_WIDTH - (master->map_width*TILESIZE))/2 - TILESIZE/2;
  master->map_offset_y = 20;
  
  return 0;
}

/* Destroy a map */
void vvg_free_map(Master* master) {

  vvl_cap_exit(&master->cap_root);
  vvl_player_exit(&master->player_root);

}

int vvg_play_game(Master* master) {

  char status = 0;
  unsigned char stuck_players;
  SDL_Event event;

  vvx_draw_all_caps(master);
  SDL_Flip(master->stdscr);

  for (;;) {
    stuck_players = 0;
    for (master->current_player = master->player_root->next;
         master->current_player != NULL;
         master->current_player = master->current_player->next) {

      /* Skips the player in case he is stuck: */
      if (vvg_event_ai(master, 1)) {
        stuck_players++;
        continue;
      }
      
      if (master->current_player->is_player)
        do
          switch (status = vvg_event_human(master, &event)) {
            case 2: /* Fall through */
            case 1: SDL_Flip(master->stdscr); break;
            case -1: return 0; break;
          }
        while (status != 2);

      else {
        vvg_event_ai(master, 0);
        SDL_Flip(master->stdscr);
      }
    }
    vvg_sanity_check(master);


    
    /* If everyone is stuck: */
    if (stuck_players == master->players) {
      printf("Game over! Press escape to close game\n");
      for (;;)
        while (SDL_WaitEvent(&event))
          if (event.type == SDL_KEYDOWN)
            if (event.key.keysym.sym == SDLK_ESCAPE)
              return 0;
    }
  }
  
  return 0;
}

int vvg_event_human(Master* master, SDL_Event* event) {

  unsigned short mouse_cap_x, mouse_cap_y;
  
  while (SDL_WaitEvent(event)) {
    if (event->type == SDL_KEYDOWN) {
      switch (event->key.keysym.sym) {
        case SDLK_1: vvg_find_caps_by_color(master, 1, 1); return 1; break;
        case SDLK_2: vvg_find_caps_by_color(master, 2, 1); return 1; break;
        case SDLK_3: vvg_find_caps_by_color(master, 3, 1); return 1; break;
        case SDLK_4: vvg_find_caps_by_color(master, 4, 1); return 1; break;
        case SDLK_5: vvg_find_caps_by_color(master, 5, 1); return 1; break;
        case SDLK_6: vvg_find_caps_by_color(master, 6, 1); return 1; break;
        case SDLK_0: vvx_hide_hoverlist(master); vvg_free_hoverlist(master); return 1; break;
        case SDLK_RETURN: return vvg_event_human_capture(master); break;
        case SDLK_ESCAPE: return -1; break;
        default: continue; break;
      }
    }
    else if (event->type == SDL_MOUSEMOTION) {
      if (event->motion.x >= master->map_offset_x
          && event->motion.x <= master->map_offset_x + (master->map_width+1)*TILESIZE
          && event->motion.y >= master->map_offset_y
          && event->motion.y <= master->map_offset_y + (master->map_height+1)*TILESIZE) {

        mouse_cap_x = (event->motion.x - master->map_offset_x) / TILESIZE;
        mouse_cap_y = (event->motion.y - master->map_offset_y) / TILESIZE;

        if (mouse_cap_x != master->mousecap_x || mouse_cap_y != master->mousecap_y) {
          master->mousecap_x = mouse_cap_x;
          master->mousecap_y = mouse_cap_y;
          return (!vvg_event_human_mouse_new_focus(master));
        }
      }
    }
    else if (event->type == SDL_MOUSEBUTTONDOWN)
      return vvg_event_human_capture(master);
    else return 0;
  }
  return 0;
}

int vvg_event_human_capture(Master* master) {

  Player *pl_p = NULL;
  
  for (pl_p = master->player_root; pl_p != NULL; pl_p = pl_p->next)
    if (master->current_player->hover_color == pl_p->color
        && master->current_player != pl_p) {
      return 0;
    }
  if (vvg_capture_hovercaps(&master->current_player) == 1) {
    return 0;
  }
  vvx_draw_capture(master);
  return 2;

}

int vvg_event_human_mouse_new_focus(Master* master) {

  Cap *cap_p = NULL;
  Cap *cap_p2 = NULL;
  
  for (cap_p = master->cap_root->next; cap_p != NULL; cap_p = cap_p->next)
    if (cap_p->x == master->mousecap_x && cap_p->y == master->mousecap_y)
      for (cap_p2 = master->current_player->cap_list; cap_p2 != NULL; cap_p2 = cap_p2->next)
        if ((cap_p2->x == cap_p->x
             && (cap_p2->y == cap_p->y-1 || cap_p2->y == cap_p->y+1)) ||
            (cap_p2->y == cap_p->y
             && (cap_p2->x == cap_p->x-1 || cap_p2->x == cap_p->x+1))) {
          vvg_find_caps_by_color(master, cap_p->color, 1);
          return 0;
        }
  if (master->current_player->hover_list != NULL) {
    vvx_hide_hoverlist(master);
    vvg_free_hoverlist(master);
    return 0;
  }
  return 1;
}

int vvg_event_ai(Master* master, int check_only) {

  char top_c, curr_c;
  short top_s = 0, curr_s;
  
  for (curr_c = 1; curr_c <= 6; curr_c++) {
    curr_s = vvg_find_caps_by_color(master, curr_c, 0);
    if (curr_s > top_s) {
      top_s = curr_s;
      top_c = curr_c;
    }
  }
  
  if (top_s == 0)
    return 1;
  else if (check_only)
    return 0;

  vvg_find_caps_by_color(master, top_c, 0);
  vvg_capture_hovercaps(&master->current_player);
  vvx_draw_capture(master);
  
  return 0;
}

int vvg_find_caps_by_color(Master* master, int color, int xupdate) {

  short found_caps;
  
  if (xupdate)
    vvx_hide_hoverlist(master);

  master->current_player->hover_color = color;
  found_caps = vvg_find_nearby_caps(master);
  if (xupdate)
    vvx_draw_hoverlist(master, found_caps);

  if (found_caps < 0) {
    fprintf(stderr, "vvg_find_caps_by_color: vvg_find_nearby_caps return -1!\n");
    return 0;
  }
  return found_caps;
}

int vvg_find_nearby_caps(Master* master) {

  Player *pl_ptr = NULL;
  
  short found_caps = 0;
  short curr_caps;
  char is_colorblocked = 0;
  Cap *cap_ptr = NULL; /* List of free caps */
  Cap *cl_ptr = NULL;  /* Iteration */
  Cap *hl_ptr = NULL;  /* List of hovercaps */

  for (pl_ptr = master->player_root->next; pl_ptr != NULL; pl_ptr = pl_ptr->next) {
    if (pl_ptr->color == master->current_player->hover_color
        && pl_ptr != master->current_player) {
      is_colorblocked = 1;
      break;
    }
  }
  
  pl_ptr = master->current_player;

  vvg_free_hoverlist(master);

  /* Find caps that are close to player's caps: */
  for (cap_ptr = master->cap_root->next; cap_ptr != NULL; cap_ptr = cap_ptr->next) {
    if (cap_ptr->color == pl_ptr->hover_color) {
      
      curr_caps = found_caps;
      cl_ptr = pl_ptr->cap_list->next;

      while (cl_ptr != NULL && curr_caps == found_caps) {
        if ((cap_ptr->y == cl_ptr->y
             && (cap_ptr->x == cl_ptr->x-1 || cap_ptr->x == cl_ptr->x+1)) ||
            (cap_ptr->x == cl_ptr->x
             && (cap_ptr->y == cl_ptr->y-1 || cap_ptr->y == cl_ptr->y+1))) {

          found_caps++;

          if (cap_ptr != master->cap_root->next) hl_ptr = cap_ptr->prev;
          else hl_ptr = master->cap_root;
          
          if (vvl_cap_move(&master->cap_root, &pl_ptr->hover_list, cap_ptr) == 1)
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
  for (cap_ptr = master->cap_root->next; cap_ptr != NULL; cap_ptr = cap_ptr->next) {
    if (cap_ptr->color == pl_ptr->hover_color) {

      curr_caps = found_caps;
      cl_ptr = pl_ptr->hover_list->next;

      while (cl_ptr != NULL && curr_caps == found_caps) {
        if ((cap_ptr->y == cl_ptr->y
             && (cap_ptr->x == cl_ptr->x-1 || cap_ptr->x == cl_ptr->x+1)) ||
            (cap_ptr->x == cl_ptr->x
             && (cap_ptr->y == cl_ptr->y-1 || cap_ptr->y == cl_ptr->y+1))) {

          found_caps++;
          
          if (cap_ptr != master->cap_root->next) hl_ptr = cap_ptr->prev;
          else hl_ptr = master->cap_root;
          
          if (vvl_cap_move(&master->cap_root, &pl_ptr->hover_list, cap_ptr) == 1)
            return -1;
          cap_ptr = master->cap_root->next;

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

void vvg_free_hoverlist(Master* master) {

  if (master->current_player->hover_list->next == NULL)
    return;

  vvl_cap_move_all(&master->current_player->hover_list, &master->cap_root);
}
  
int vvg_capture_hovercaps(Player** player) {

  if ((*player)->hover_list->next == NULL)
    return 1;

  vvl_cap_move_all(&(*player)->hover_list, &(*player)->cap_list);
  (*player)->color = (*player)->hover_color;
  (*player)->hover_color = 0;

  return 0;
}

int vvg_sanity_check(Master* master) {

  short ccap = 0, ccl = 0, chl = 0, cinst = 0;
  Cap *cap_ptr = NULL;
  Player *pl_ptr = NULL;

  for (cap_ptr = master->cap_root->next; cap_ptr != NULL; cap_ptr = cap_ptr->next, ccap++);
  for (pl_ptr = master->player_root->next; pl_ptr != NULL; pl_ptr = pl_ptr->next) {
    for (cap_ptr = pl_ptr->cap_list->next; cap_ptr != NULL; cap_ptr = cap_ptr->next, ccl++);
    for (cap_ptr = pl_ptr->hover_list->next; cap_ptr != NULL; cap_ptr = cap_ptr->next, chl++);
  }

  cinst = ccap + ccl + chl;

  if (!(cinst == master->instances)) {
    fprintf(stderr, "Missing instances! Was %d, is %d (%d-%d-%d)\n",
            master->instances, cinst, ccap, ccl, chl);

    for (pl_ptr = master->player_root->next; pl_ptr != NULL; pl_ptr = pl_ptr->next) {
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
