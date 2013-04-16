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
  *buf = malloc(sizeof(char)*vvk_get_filesize(fd));
  
  fgets(minibuf, sizeof minibuf, fd);
  strcpy(*buf, minibuf);
  while ( (fgets(minibuf, sizeof minibuf, fd)) != NULL)
    strcat(*buf, minibuf);

  fclose(fd);
  free(fullpath);
  
  return 0;
}

int vvk_make_map(char** map_buffer, Cap** cap_root, Player** player_root) {

  int x, y,  players = 2;
  char *p = NULL;
  Cap *cap_ptr = NULL, *cap_killer = NULL;
  Player *player_ptr = NULL, *player_killer = NULL;
  
  srand(time(0));

  /* Iterate through the map:
   '#' becomes a Cap (game tile) and goes into a cap_root linked list
   '@' becomes a player and goes into player_root linked list
  */
  *cap_root = (Cap *)malloc(sizeof(Cap));
  (*cap_root)->x = -1;
  (*cap_root)->y = -1;
  (*cap_root)->is_active = 0;
  (*cap_root)->color = 0;
  (*cap_root)->next = NULL;
  cap_ptr = cap_root[0];

  *player_root = (Player *)malloc(sizeof(Player));
  (*player_root)->symbol = -1;
  (*player_root)->color = -1;
  (*player_root)->is_player = -1;
  (*player_root)->cap_list = NULL;
  player_ptr = player_root[0];

  for(p = map_buffer[0], x = 0, y = 0; *p != '\0'  ; p++, x++) {
    if (*p == '\n') {
      y++;
      x = -1;
    }
    else if (*p == '#') {
      cap_ptr->next = (Cap *)malloc(sizeof(Cap));
      cap_ptr = cap_ptr->next;
      cap_ptr->x = x;
      cap_ptr->y = y;
      cap_ptr->is_active = 1;
      cap_ptr->color = rand() % 6 + 1; 
      cap_ptr->next = NULL;
    }
    else if (*p == '@') {
      player_ptr->next = (Player *)malloc(sizeof(Player));
      player_ptr = player_ptr->next;
      player_ptr->symbol = players++;
      player_ptr->color = 0;
      if (players == 3)
        player_ptr->is_player = 1;
      else
        player_ptr->is_player = 0;

      player_ptr->cap_list = (CapList *)malloc(sizeof(CapList));
      player_ptr->cap_list->x = x;
      player_ptr->cap_list->y = y;
      player_ptr->cap_list->next = NULL;

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
  player_ptr->cap_list = player_killer->cap_list;
  player_ptr->next = player_killer->next;

  free(player_killer);

  
  cap_ptr = cap_root[0];
  cap_killer = cap_ptr->next;
  
  cap_ptr->x = cap_killer->x;
  cap_ptr->y = cap_killer->y;
  cap_ptr->is_active = cap_killer->is_active;
  cap_ptr->color = cap_killer->color;
  cap_ptr->next = cap_killer->next;

  free(cap_killer);

  /* Free map_buffer: */
  free(*map_buffer);
  *map_buffer = NULL;
  
  return 0;
}

void vvk_free_map(Cap** cap_root, Player** player_root) {

  Cap *cap_ptr = cap_root[0], *cap_killer = NULL;
  Player *player_ptr = player_root[0], *player_killer = NULL;
  CapList *cap_list_ptr = NULL, *cap_list_killer = NULL;

  while (cap_ptr != NULL) {
    cap_killer = cap_ptr;
    cap_ptr = cap_ptr->next;
    
    free(cap_killer);
    cap_killer = NULL;
  }

  while (player_ptr != NULL) {
    player_killer = player_ptr;
    player_ptr = player_ptr->next;

    cap_list_ptr = player_killer->cap_list;
    while (cap_list_ptr != NULL) {
      cap_list_killer = cap_list_ptr;
      cap_list_ptr = cap_list_ptr->next;
      free(cap_list_killer);
      cap_list_killer = NULL;
    }

    free(player_killer);
    player_killer = NULL;
  }
}

int vvk_play_game(SDL_Surface** stdscr, SDL_Surface** imgscr,
                  Cap** cap_root, Player** player_root) {

  vvk_draw_all_caps(stdscr, imgscr, cap_root, player_root);
  SDL_Flip(*stdscr);

  
  
  return 0;
}
