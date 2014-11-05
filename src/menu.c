#include "menu.h"

#include <SDL/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include "map.h"
#include "graphics.h"

static void
play()
  {
    map_exit();
  }

static void
down()
  {
  }

static void
up()
  {
  }

#ifdef __EMSCRIPTEN__
static void
#else
static int
#endif
main_menu_iteration()
  {
    SDL_Event event;
    SDL_WaitEvent(&event);
    if (event.type == SDL_KEYDOWN)
      {
        switch (event.key.keysym.sym)
          {
#ifndef __EMSCRIPTEN__
            case SDLK_ESCAPE: return 0;
#endif
            case SDLK_RETURN: play();
            case SDLK_DOWN:   down();
            case SDLK_UP:     up();
            default:          break;
          }
      }
#ifndef __EMSCRIPTEN__
    return 1;
#endif
  }

void
main_menu()
  {
    if (map_init() != 0)
      {
        return;
      }

    graphics_paint_screen_black();
    graphics_print_text(CAPTION, 30);

    int y = 50;
    int num_maps = map_num_maps();
    for (int i = 0; i < num_maps; ++i)
      {
        graphics_print_text(map_get_map_name(i), y + i * 20);
      }
    graphics_draw_updates();

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_menu_iteration, 0, 1);
#else
    while (main_menu_iteration());
#endif

    map_exit();
  }
