#include "graphics.h"

#include <stdio.h>

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"

static SDL_Surface *stdscr = NULL;
static SDL_Surface *imgscr = NULL;
static TTF_Font *stdfont = NULL;

static SDL_Surface *
_make_text(const char *text, SDL_Color *color, SDL_Color *background)
  {
    SDL_Surface *temp_surface =
      TTF_RenderText_Shaded(stdfont, text, *color, *background);

    if (temp_surface == NULL)
      {
        fprintf(stderr, "TTF_RenderText_Shaded failed: %s\n", TTF_GetError());
      }

    return temp_surface;
  }

static int
_blit(SDL_Surface *s_from, SDL_Rect *r_from, SDL_Surface *s_to, SDL_Rect *r_to)
  {
    if (SDL_BlitSurface(s_from, r_from, s_to, r_to) != 0)
      {
        fprintf(stderr, "SDL_BlitSurface failed: %s\n", SDL_GetError());
        return 1;
      }
    return 0;
  }

int
graphics_init()
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

void
graphics_exit()
  {
    TTF_CloseFont(stdfont);
    stdfont = NULL;
    TTF_Quit();

    SDL_FreeSurface(imgscr);
    imgscr = NULL;
    SDL_Quit();
    stdscr = NULL;
  }

int
graphics_paint_screen_black()
  {
    return SDL_FillRect(stdscr, NULL, SDL_MapRGB(stdscr->format, 0, 0, 0)) != 0;
  }

int graphics_print_text(const char *text, int height)
  {
    SDL_Color color = { 255, 127, 0, 0 };  /* Orange */
    SDL_Color background = { 0, 0, 0, 0 }; /* Black  */

    SDL_Surface *temp_surface = _make_text(text, &color, &background);
    if (temp_surface == NULL)
      {
        return 1;
      }

    SDL_Rect rect = { stdscr->w/2 - temp_surface->w/2, height, 0, 0 };
    int status = _blit(temp_surface, NULL, stdscr, &rect);
    SDL_FreeSurface(temp_surface);

    return status;
  }

int
graphics_draw_updates()
  {
    return SDL_Flip(stdscr) != 0;
  }
