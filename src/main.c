
/* Rune Warz
 Created 2013-04-08 by Olle K
 Image(s) by Sofie Aid

 Made as a port from python to C
*/

#include <stdio.h>
#include <unistd.h>

#include "SDL/SDL.h"

#include "vvl_link.h"
#include "vvx_graphics.h"
#include "vvg_game.h"

int main(void) {
  SDL_Surface *stdscr = NULL, *imgscr = NULL;
  int instances = 0;
  char *map_buffer = NULL;
  Cap *cap_root = NULL;
  Player *player_root = NULL;

  if (vvx_init(&stdscr, &imgscr) == 1) return 1;
  

  if (vvg_load_mapfile("map2_2", &map_buffer) == 1) return 1;
  if (vvg_make_map(&map_buffer, &cap_root, &player_root, &instances) == 1) return 1;

  vvg_play_game(&stdscr, &imgscr, &cap_root, &player_root, instances);
  
  
  vvg_free_map(&cap_root, &player_root);
  vvx_exit(&imgscr);
  return 0;
}
