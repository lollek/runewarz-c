
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

  Master master;
  int status;
  
  memset(&master, 0, sizeof(master));
  
  if (vvg_load_mapfile(&master, "map1_4") == 1) return 1;
  
  status = vvg_make_map(&master);
  if (status == 1) return 1;
  else if (status == 0) {
    if (vvx_init(&master) == 1) return 1;
    vvg_play_game(&master);
  }
  
  vvg_free_map(&master);
  vvx_exit(&master);
  return 0;
}
