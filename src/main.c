
/* Rune Warz
 Created 2013-04-08 by Olle K
 Image(s) by Sofie Aid

 Made as a port from python to C
*/

#include <stdio.h>
#include <unistd.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"

#include "vvk_graphics.h"
#include "vvk_game.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_BPP 32
#define TILESIZE 15

#define CAPTION "RuneWarz v0.1"

int init_graphics(SDL_Surface** stdscr, SDL_Surface** imgscr) {

  SDL_Surface *imgscr_temp;

  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    fprintf(stderr, "Failed to init sdl\n");
    return 1;
  }

  *stdscr = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
  if (*stdscr == NULL) {
    fprintf(stderr, "Failed to set videomode\n");
    return 1;
    }

  if ((imgscr_temp = IMG_Load("img/tiles.png")) == NULL) {
    fprintf(stderr, "Failed to load img/tiles.png\n");
    return 1;
  }
  if ( (*imgscr = SDL_DisplayFormat(imgscr_temp)) == NULL) {
    fprintf(stderr, "Failed to format img/tiles.png\n");
    return 1;
  }
  SDL_FreeSurface(imgscr_temp);

  SDL_WM_SetCaption(CAPTION, CAPTION);

  return 0;
}

void exit_graphics(SDL_Surface** imgscr) {
  SDL_FreeSurface(*imgscr);
  /* SDL_FreeSurface(*stdscr); */
  SDL_Quit();
}

int main(void) {
  SDL_Surface *stdscr = NULL, *imgscr = NULL;
  int instances = 0;
  char *map_buffer = NULL;
  Cap *cap_root = NULL;
  Player *player_root = NULL;

  if (init_graphics(&stdscr, &imgscr) == 1)
    return 1;
  

  if (vvk_load_mapfile("map2_2", &map_buffer) == 1)
    return 1;
  vvk_make_map(&map_buffer, &cap_root, &player_root, &instances);
  fprintf(stderr, "%d+1 instances", --instances);

  vvk_play_game(&stdscr, &imgscr, &cap_root, &player_root, instances);
  
  
  vvk_free_map(&cap_root, &player_root);
  exit_graphics(&imgscr);
  return 0;
}
