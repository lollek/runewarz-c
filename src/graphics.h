#ifndef RUNEWARZ_GRAPHICS_H
#define RUNEWARZ_GRAPHICS_H

#include <stdio.h>

#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"

#define TILES_PATH "img/tiles.png"
#define FONT_PATH "img/freemonobold.ttf"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_BPP 24
#define TILESIZE 15
#define SYMBOL_OFFSET 15

#define CAPTION "RuneWarz v0.2.1"

/* Start up SDL and its subsystems, then load all images from disk
 * Returns 0 on success and automatically prints the error */
int init_graphics();

#endif /* RUNEWARZ_GRAPHICS_H */
