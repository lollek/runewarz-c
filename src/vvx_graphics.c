#include "vvx_graphics.h"

int vvx_init(Master* master) {
  
  SDL_Surface *imgscr_temp;
  
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    fprintf(stderr, "Failed to init sdl\n");
    return 1;
  }
  
  (*master).stdscr = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
  if ((*master).stdscr == NULL) {
    fprintf(stderr, "Failed to set videomode\n");
    return 1;
  }
  
  if ((imgscr_temp = IMG_Load("img/tiles.png")) == NULL) {
    fprintf(stderr, "Failed to load img/tiles.png\n");
    return 1;
  }
  if (((*master).imgscr = SDL_DisplayFormat(imgscr_temp)) == NULL) {
    fprintf(stderr, "Failed to format img/tiles.png\n");
    return 1;
  }
  SDL_FreeSurface(imgscr_temp);

  SDL_WM_SetCaption(CAPTION, CAPTION);

  return 0;

}

void vvx_exit(Master* master) {
  SDL_FreeSurface((*master).imgscr);
  SDL_Quit();
}

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

void vvx_draw_all_caps(Master* master) {

  Cap *cap_ptr = (*master).cap_root->next;
  Player *player_ptr = (*master).player_root->next;
  Cap *cap_list_ptr = NULL;

  int offset_x = (SCREEN_WIDTH - ((*master).map_width*TILESIZE))/2;
  int offset_y = 20;
  int scoreboard_y = 400;
  int scoreboard_x = offset_x;

  SDL_Rect source, target;
  source.w = TILESIZE; source.h = TILESIZE;

  /* Draw all free caps: */
  while (cap_ptr != NULL) {
    source.x = cap_ptr->color*TILESIZE;
    source.y = 0;
    
    target.x = cap_ptr->x*TILESIZE + offset_x;
    target.y = cap_ptr->y*TILESIZE + offset_y;
    SDL_BlitSurface((*master).imgscr, &source, (*master).stdscr, &target);
    
    cap_ptr = cap_ptr->next;
  }

  /* Draw all free players: */
  while (player_ptr != NULL) {
    cap_list_ptr = player_ptr->cap_list->next;

    source.w = TILESIZE;
    source.x = player_ptr->color*TILESIZE;
    source.y = player_ptr->symbol*TILESIZE + SYMBOL_OFFSET;

    target.x = scoreboard_x; scoreboard_x += TILESIZE;
    target.y = scoreboard_y + player_ptr->symbol*20;
    SDL_BlitSurface((*master).imgscr, &source, (*master).stdscr, &target);
    
    while (cap_list_ptr != NULL) {

      source.w = TILESIZE;
      target.x = cap_list_ptr->x*TILESIZE + offset_x;
      target.y = cap_list_ptr->y*TILESIZE + offset_y;
      SDL_BlitSurface((*master).imgscr, &source, (*master).stdscr, &target);

      source.w = 1;
      target.x = scoreboard_x; scoreboard_x += 1;
      target.y = scoreboard_y + player_ptr->symbol*20;
      SDL_BlitSurface((*master).imgscr, &source, (*master).stdscr, &target);
      
      cap_list_ptr = cap_list_ptr->next;
    }

    scoreboard_x = offset_x;
    player_ptr = player_ptr->next;
  }

}
void vvx_draw_capture(Master* master) {
  
  Cap *cap_p = NULL;

  int offset_x = (SCREEN_WIDTH - ((*master).map_width*TILESIZE))/2;
  int offset_y = 20;
  int scoreboard_y = 400;
  int scoreboard_x = offset_x;

  SDL_Rect source, target;
  source.x = (*master).current_player->color*TILESIZE;
  source.y = SYMBOL_OFFSET + (*master).current_player->symbol*TILESIZE;
  source.w = TILESIZE; source.h = TILESIZE;

  target.x = scoreboard_x; scoreboard_x += 15;
  target.y = scoreboard_y + (*master).current_player->symbol*20;
  SDL_BlitSurface((*master).imgscr, &source, (*master).stdscr, &target);

  for (cap_p = (*master).current_player->cap_list->next; cap_p != NULL; cap_p = cap_p->next) {

    source.w = TILESIZE;
    target.x = cap_p->x*TILESIZE + offset_x;
    target.y = cap_p->y*TILESIZE + offset_y;
    SDL_BlitSurface((*master).imgscr, &source, (*master).stdscr, &target);

    source.w = 1;
    target.x = scoreboard_x; scoreboard_x += 1;
    target.y = scoreboard_y + (*master).current_player->symbol*20;
    SDL_BlitSurface((*master).imgscr, &source, (*master).stdscr, &target);
  }
}

void vvx_draw_hoverlist(Master* master, int pad) {
  
  Cap *cap_p = NULL;

  int offset_x = (SCREEN_WIDTH - ((*master).map_width*TILESIZE))/2;
  int offset_y = 20;

  SDL_Rect source, target;
  source.x = (*master).current_player->hover_color*TILESIZE;
  if (pad) source.y = SYMBOL_OFFSET + (*master).current_player->symbol*TILESIZE;
  else source.y = TILESIZE;
  source.w = TILESIZE; source.h = TILESIZE;
  target.w = TILESIZE; target.h = TILESIZE;

  if ((*master).current_player->hover_list->next == NULL)
    return;

  for (cap_p = (*master).current_player->hover_list->next; cap_p != NULL; cap_p = cap_p->next){
    target.x = cap_p->x*TILESIZE + offset_x;
    target.y = cap_p->y*TILESIZE + offset_y;

    SDL_BlitSurface((*master).imgscr, &source, (*master).stdscr, &target);
  }
}

void vvx_hide_hoverlist(Master* master) {

  Cap *cap_p = NULL;

  int offset_x = (SCREEN_WIDTH - ((*master).map_width*TILESIZE))/2;
  int offset_y = 20;

  SDL_Rect source, target;
  source.x = (*master).current_player->hover_color*TILESIZE;
  source.y = 0;
  source.w = TILESIZE; source.h = TILESIZE;
  target.w = TILESIZE; target.h = TILESIZE;

  if ((*master).current_player->hover_list->next == NULL)
    return;

  for (cap_p = (*master).current_player->hover_list->next; cap_p != NULL; cap_p = cap_p->next){
    
    target.x = cap_p->x*TILESIZE + offset_x;
    target.y = cap_p->y*TILESIZE + offset_y;

    SDL_BlitSurface((*master).imgscr, &source, (*master).stdscr, &target);
  }
}
