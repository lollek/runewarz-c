#include "graphics.h"

static SDL_Surface *stdscr = NULL;
static SDL_Surface *imgscr = NULL;
static TTF_Font *stdfont = NULL;

int graphics_init()
  {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
      {
        fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
      }

    if (TTF_Init() == -1)
      {
        fprintf(stderr, "Failed to initialize TTF: %s\n", TTF_GetError());
        return 1;
      }

    stdscr = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
    if (stdscr == NULL)
      {
        fprintf(stderr, "Failed to create main SDL surface: %s\n", SDL_GetError());
        return 1;
      }

    imgscr = IMG_Load(TILES_PATH);
    if (imgscr == NULL)
      {
        fprintf(stderr, "Failed to load tiles for SDL: %s\n", SDL_GetError());
        return 1;
      }

#ifndef __EMSCRIPTEN__ /* SDL_DisplayFormat does not exist in emscripten */
    SDL_Surface *imgscr_temp = imgscr;
    imgscr = SDL_DisplayFormat(imgscr_temp);
    if (imgscr == NULL)
      {
        fprintf(stderr, "Failed to optimise tiles: %s\n", SDL_GetError());
        imgscr = imgscr_temp;
      }
    else
      {
        SDL_FreeSurface(imgscr_temp);
      }
#endif /* __EMSCRIPTEN__ */

    stdfont = TTF_OpenFont(FONT_PATH, TILESIZE);
    if (stdfont == NULL)
      {
        fprintf(stderr, "Failed to load TTF font: %s\n", TTF_GetError());
        return 1;
      }

    SDL_WM_SetCaption(CAPTION, CAPTION);

    return 0;
  }

void graphics_exit()
  {
    TTF_CloseFont(stdfont);
    stdfont = NULL;
    TTF_Quit();

    SDL_FreeSurface(imgscr);
    imgscr = NULL;
    SDL_Quit();
    stdscr = NULL;
  }
