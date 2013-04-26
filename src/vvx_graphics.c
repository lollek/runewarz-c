#include "vvx_graphics.h"

int vvx_init(Master* master) {

  SDL_Surface *imgscr_temp;
  
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    fprintf(stderr, "Failed to initialize SDL: %s\n", SDL_GetError());
    return 1;
  }

  if (TTF_Init() == -1) {
    fprintf(stderr, "Failed to initialize TTF: %s\n", TTF_GetError());
    return 1;
  }
  
  (*master).stdscr = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
  if ((*master).stdscr == NULL) {
    fprintf(stderr, "Failed to create main SDL surface: %s\n", SDL_GetError());
    return 1;
  }
  
  if ((imgscr_temp = IMG_Load("img/tiles.png")) == NULL) {
    fprintf(stderr, "Failed to load tiles for SDL: %s\n", SDL_GetError());
    return 1;
  }
  if (((*master).imgscr = SDL_DisplayFormat(imgscr_temp)) == NULL) {
    fprintf(stderr, "Failed to format tiles for SDL: %s\n", SDL_GetError());
    return 1;
  }

  if (((*master).font = TTF_OpenFont("img/freemonobold.ttf", 16)) == NULL) {
    fprintf(stderr, "Failed to load TTF font: %s\n", TTF_GetError());
    return 1;
  }
  
  SDL_FreeSurface(imgscr_temp);

  SDL_WM_SetCaption(CAPTION, CAPTION);

  return 0;

}

void vvx_exit(Master* master) {
  TTF_CloseFont((*master).font);
  SDL_FreeSurface((*master).imgscr);
  TTF_Quit();
  SDL_Quit();
}


void vvx_draw_text(Master* master, const char text[], int x, int y, int is_c, int clr) {

  SDL_Color text_color = { 0, 0, 0, 0};
  const SDL_Color bg_color = { 0, 0, 0, 0 };
  SDL_Surface *text_surface = NULL;
  SDL_Rect text_rect;

  if (clr == 1) /* Blue */ {
    text_color.b = 255;
  } else if (clr == 2) /* Green */ {
    text_color.g = 255;
  } else if (clr == 3) /* Orange*/ {
    text_color.r = 255;
    text_color.g = 127;
  } else if (clr == 4) /* Pink */ {
    text_color.r = 255;
    text_color.g = 192;
    text_color.b = 203;
  } else if (clr == 5) /* Grey */ {
    text_color.r = 128;
    text_color.g = 128;
    text_color.b = 128;
  } else if (clr == 6) /* Red */ {
    text_color.r = 255;
  } else if (clr == 7) /* Olive */ {
    text_color.r = 128;
    text_color.g = 128;
  } else if (clr == 8) /* Cyan */ {
    text_color.g = 255;
    text_color.b = 255;
  }
    
  text_surface = TTF_RenderText_Shaded((*master).font, text, text_color, bg_color);

  if (is_c) text_rect.x = x - text_surface->w/2;
  else text_rect.x = x;
  text_rect.y = y;

  SDL_BlitSurface(text_surface, NULL, (*master).stdscr, &text_rect);
  SDL_FreeSurface(text_surface);
  
}

/*
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
*/

void vvx_draw_all_caps(Master* master) {

  Cap *cap_ptr = (*master).cap_root->next;
  Player *player_ptr = (*master).player_root->next;
  Cap *cap_list_ptr = NULL;

  int offset_x = (*master).map_offset_x;
  int offset_y = (*master).map_offset_y;
  int scoreboard_x = offset_x;
  int cap_counter = 0;
  char cap_counter_str[4];

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
    target.y = (*master).map_height*TILESIZE + offset_y*2 + player_ptr->symbol*20;
    SDL_BlitSurface((*master).imgscr, &source, (*master).stdscr, &target);
    
    while (cap_list_ptr != NULL) {

      cap_counter++;
      
      source.w = TILESIZE;
      target.x = cap_list_ptr->x*TILESIZE + offset_x;
      target.y = cap_list_ptr->y*TILESIZE + offset_y;
      SDL_BlitSurface((*master).imgscr, &source, (*master).stdscr, &target);

      source.w = 1;
      target.x = scoreboard_x; scoreboard_x += 1;
      target.y = (*master).map_height*TILESIZE + offset_y*2 + player_ptr->symbol*20;
      SDL_BlitSurface((*master).imgscr, &source, (*master).stdscr, &target);
      
      cap_list_ptr = cap_list_ptr->next;
    }

    sprintf(cap_counter_str, "%d", cap_counter);
    vvx_draw_text(master, cap_counter_str, offset_x - 20, target.y, 1, player_ptr->color);

    scoreboard_x = offset_x;
    player_ptr = player_ptr->next;
  }

}
void vvx_draw_capture(Master* master) {
  
  Cap *cap_p = NULL;

  int offset_x = (*master).map_offset_x;
  int offset_y = (*master).map_offset_y;
  int scoreboard_x = offset_x;
  int cap_counter = 0;
  char cap_counter_str[4];

  SDL_Rect source, target;
  source.x = (*master).current_player->color*TILESIZE;
  source.y = SYMBOL_OFFSET + (*master).current_player->symbol*TILESIZE;
  source.w = TILESIZE; source.h = TILESIZE;

  target.x = scoreboard_x; scoreboard_x += 15;
  target.y = (*master).map_height*TILESIZE + offset_y*2 + (*master).current_player->symbol*20;
  SDL_BlitSurface((*master).imgscr, &source, (*master).stdscr, &target);

  for (cap_p = (*master).current_player->cap_list->next; cap_p != NULL; cap_p = cap_p->next) {

    cap_counter++;
    
    source.w = TILESIZE;
    target.x = cap_p->x*TILESIZE + offset_x;
    target.y = cap_p->y*TILESIZE + offset_y;
    SDL_BlitSurface((*master).imgscr, &source, (*master).stdscr, &target);

    source.w = 1;
    target.x = scoreboard_x; scoreboard_x += 1;
    target.y = (*master).map_height*TILESIZE + offset_y*2 +
      (*master).current_player->symbol*20;
    SDL_BlitSurface((*master).imgscr, &source, (*master).stdscr, &target);
  }


  sprintf(cap_counter_str, "%d", cap_counter);
  vvx_draw_text(master, cap_counter_str, offset_x - 20, target.y, 1,
                (*master).current_player->color);

}

void vvx_draw_hoverlist(Master* master, int pad) {
  
  Cap *cap_p = NULL;

  int offset_x = (*master).map_offset_x;
  int offset_y = (*master).map_offset_y;

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

  int offset_x = (*master).map_offset_x;
  int offset_y = (*master).map_offset_y;

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
