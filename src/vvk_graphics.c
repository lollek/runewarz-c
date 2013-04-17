#include "vvk_graphics.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TILESIZE 15

void vvk_render_text_centered(SDL_Surface** target_surface, TTF_Font** font, const char text[],
                              SDL_Color text_color, int offset_x, int offset_y) {
  SDL_Surface *text_surf = NULL;
  SDL_Rect text_rect;

  text_surf = TTF_RenderText_Blended(*font, text, text_color);
  text_rect.x = (*target_surface)->w/2 - text_surf->w/2 + offset_x;
  text_rect.y = (*target_surface)->h/2 - text_surf->h/2 + offset_y;
  SDL_BlitSurface(text_surf, NULL, *target_surface, &text_rect);
  SDL_FreeSurface(text_surf);
}

void vvk_render_text_absolute(SDL_Surface** target_surface, TTF_Font** font, const char text[],
                              SDL_Color text_color, int offset_x, int offset_y) {
  SDL_Surface *text_surf = NULL;
  SDL_Rect text_rect;

  text_surf = TTF_RenderText_Blended(*font, text, text_color);
  text_rect.x = offset_x;
  text_rect.y = offset_y;
  SDL_BlitSurface(text_surf, NULL, *target_surface, &text_rect);
  SDL_FreeSurface(text_surf);
}

void vvk_render_box_centered(SDL_Surface** target_surface,
                             int offset_x, int offset_y, int width, int height) {

  SDL_Surface *temp_surf = NULL;
  SDL_Rect temp_rect;

  temp_surf = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0, 0, 0, 0);
  temp_rect.x = 1; temp_rect.w = temp_surf->w-2;
  temp_rect.y = 1; temp_rect.h = temp_surf->h-2;
  SDL_FillRect(temp_surf, &temp_rect, SDL_MapRGB(temp_surf->format, 100, 100, 100));
  temp_rect.w = 0; temp_rect.h = 0;

  temp_rect.x = (*target_surface)->w/2 - temp_surf->w/2 + offset_x;
  temp_rect.y = (*target_surface)->h/2 - temp_surf->h/2 + offset_y;
  SDL_BlitSurface(temp_surf, NULL, *target_surface, &temp_rect);

  SDL_FreeSurface(temp_surf);
}

void vvk_render_box_absolute(SDL_Surface** target_surface,
                             int offset_x, int offset_y, int width, int height) {

  SDL_Surface *temp_surf = NULL;
  SDL_Rect temp_rect;

  temp_surf = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0, 0, 0, 0);
  temp_rect.x = 1; temp_rect.w = temp_surf->w-2;
  temp_rect.y = 1; temp_rect.h = temp_surf->h-2;
  SDL_FillRect(temp_surf, &temp_rect, SDL_MapRGB(temp_surf->format, 100, 100, 100));
  temp_rect.w = 0; temp_rect.h = 0;

  temp_rect.x = offset_x;
  temp_rect.y = offset_y;
  SDL_BlitSurface(temp_surf, NULL, *target_surface, &temp_rect);

  SDL_FreeSurface(temp_surf);
}

void vvk_draw_all_caps(SDL_Surface** stdscr, SDL_Surface** imgscr,
                       Cap** cap_root, Player** player_root) {

  Cap *cap_ptr = cap_root[0];
  Player *player_ptr = player_root[0];
  CapList *cap_list_ptr = NULL;

  SDL_Rect source, target;
  source.w = TILESIZE; source.h = TILESIZE;
  target.w = TILESIZE; target.h = TILESIZE;

  /* Draw all free caps: */
  while (cap_ptr != NULL) {
    source.x = cap_ptr->color*TILESIZE;
    source.y = 0;
    
    target.x = cap_ptr->x*TILESIZE;
    target.y = cap_ptr->y*TILESIZE;

    SDL_BlitSurface(*imgscr, &source, *stdscr, &target);
    cap_ptr = cap_ptr->next;
  }

  /* Draw all free players: */
  while (player_ptr != NULL) {
    cap_list_ptr = player_ptr->cap_list;
    
    while (cap_list_ptr != NULL) {
      source.x = player_ptr->color*TILESIZE;
      source.y = player_ptr->symbol*TILESIZE;

      target.x = cap_list_ptr->x*TILESIZE;
      target.y = cap_list_ptr->y*TILESIZE;

      SDL_BlitSurface(*imgscr, &source, *stdscr, &target);
      cap_list_ptr = cap_list_ptr->next;
    }
    player_ptr = player_ptr->next;
  }
}

void vvk_draw_hover(SDL_Surface** stdscr, SDL_Surface** imgscr,
                    Cap** cap_root, Player** player_root) {

  Cap *cap_ptr = cap_root[0];
  Player *player_ptr = player_root[0], *player_aux_ptr = NULL;
  CapList *cap_list_ptr = NULL;

  int cap_is_blocked = 0;

  SDL_Rect source, target;
  source.w = TILESIZE; source.h = TILESIZE;
  target.w = TILESIZE; target.h = TILESIZE;

  for (player_aux_ptr = player_root[0];
       player_aux_ptr != NULL;
       player_aux_ptr = player_aux_ptr->next) {
    if (player_aux_ptr->color == player_ptr->hover_color
        && player_aux_ptr->is_player == -1)
      cap_is_blocked = 1;
  }

  source.x = player_ptr->hover_color*TILESIZE;
  if (cap_is_blocked)
    source.y = TILESIZE;
  else
    source.y = player_ptr->symbol*TILESIZE;
  
  while (cap_ptr != NULL) {
    if (cap_ptr->color == player_ptr->hover_color) {
      for (cap_list_ptr = player_ptr->cap_list;
           cap_list_ptr != NULL;
           cap_list_ptr = cap_list_ptr->next) {

        if (cap_ptr->x == cap_list_ptr->x
            && cap_ptr->y >= cap_list_ptr->y-1
            && cap_ptr->y <= cap_list_ptr->y+1) {
          target.x = cap_ptr->x*TILESIZE;
          target.y = cap_ptr->y*TILESIZE;
          SDL_BlitSurface(*imgscr, &source, *stdscr, &target);
        }
          

        else if (cap_ptr->y == cap_list_ptr->y
                 && cap_ptr->x >= cap_list_ptr->x-1
                 && cap_ptr->x <= cap_list_ptr->x+1) {
          target.x = cap_ptr->x*TILESIZE;
          target.y = cap_ptr->y*TILESIZE;
          SDL_BlitSurface(*imgscr, &source, *stdscr, &target);
        }
      }
    }
    cap_ptr = cap_ptr->next;
  }
}

void vvk_draw_unhoover(SDL_Surface** stdscr, SDL_Surface** imgscr,
                      Cap** cap_root, Player** player_root) {

  Cap *cap_ptr = cap_root[0];
  Player *player_ptr = player_root[0];
  CapList *cap_list_ptr = NULL;

  SDL_Rect source, target;
  source.w = TILESIZE; source.h = TILESIZE;
  target.w = TILESIZE; target.h = TILESIZE;

  source.x = player_ptr->hover_color*TILESIZE;
  source.y = 0;
  
  while (cap_ptr != NULL) {
    if (cap_ptr->color == player_ptr->hover_color) {
      for (cap_list_ptr = player_ptr->cap_list;
           cap_list_ptr != NULL;
           cap_list_ptr = cap_list_ptr->next) {

        if (cap_ptr->x == cap_list_ptr->x
            && cap_ptr->y >= cap_list_ptr->y-1
            && cap_ptr->y <= cap_list_ptr->y+1) {
          target.x = cap_ptr->x*TILESIZE;
          target.y = cap_ptr->y*TILESIZE;
          SDL_BlitSurface(*imgscr, &source, *stdscr, &target);
        }
          

        else if (cap_ptr->y == cap_list_ptr->y
                 && cap_ptr->x >= cap_list_ptr->x-1
                 && cap_ptr->x <= cap_list_ptr->x+1) {
          target.x = cap_ptr->x*TILESIZE;
          target.y = cap_ptr->y*TILESIZE;
          SDL_BlitSurface(*imgscr, &source, *stdscr, &target);
        }
      }
    }
    cap_ptr = cap_ptr->next;
  }
  
}
