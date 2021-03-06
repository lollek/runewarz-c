
/** Rune Warz
 * Created 2013-04-08 by Olle K
 * Image(s) by Sofie Aid
 *
 * Made as a port from python to C
 */

#include <stdio.h>

#include "SDL/SDL.h"

#include "vvl_link.h"
#include "vvx_graphics.h"
#include "vvg_game.h"

/* If selected map is too high/low, it jumps to the other end */
void wrap_around(char avail_maps, char* selected_map) {

  if (*selected_map > avail_maps-1)
    *selected_map = 1;
  else if (*selected_map < 1)
    *selected_map = avail_maps-1;

}

int main_event(SDL_Event* event, char* selected_map) {
  while (SDL_WaitEvent(event)) {
    if (event->type == SDL_KEYDOWN) {
      switch (event->key.keysym.sym) {
        case SDLK_ESCAPE: return -1; break;
        case SDLK_RETURN: return 3; break;
        case SDLK_DOWN: (*selected_map)++; return 2; break;
        case SDLK_UP: (*selected_map)--; return 2; break;
        default: return 0; break;
      }
    }
  }
  return 0;
}

int main(void) {

  Master master;
  Map map_root, *map_p;

  SDL_Event event;

  char status;
  char selected_map = 0;
  char loop, avail_maps;

  /** Init
   * Create master instance for holding surfaces and linked-list roots
   * Init graphics
   * Create a list of found maps under maps/-folder
   * Draw main menu
  */
  memset(&master, 0, sizeof(master));
  if (vvx_init(&master) == 1) return 1;
  memset(&map_root, 0, sizeof(map_root));
  if ( (avail_maps = vvl_map_add(&map_root)) == -1) return 1;
  vvx_draw_main_menu(&master, &map_root);
  
  /** Main Loop
   * ESC:    -1 / exit
   * UP/DOWN: 2 / change selected map
   * ENTER:   3 / play
   */
  for (loop = 1; loop;) {
    switch (main_event(&event, &selected_map)) {

      case -1: {
        loop = 0;
      } break;

      case 2: { 
        wrap_around(avail_maps, &selected_map);
        vvx_update_main_menu(&master, &map_root, selected_map);
      } break;

      case 3: { 
        if (selected_map >= 1 && selected_map <= avail_maps) {
          for (map_p = map_root.next; map_p != NULL && selected_map--> 1; map_p = map_p->next);
          if (map_p != NULL) {
            status = vvg_make_map(&master, map_p->name);
            if (status  == 1) return 1;
            if (status != 0) break;

            vvg_play_game(&master);
            vvg_free_map(&master);
            selected_map = 0;
            vvx_draw_main_menu(&master, &map_root);
          }
        } break;
      }

      default: break;
    }
  }

  vvl_map_remove(&map_root);
  vvx_exit(&master);
  return 0;
}
