#include "vvx_graphics.h"

void vvx_draw_main_menu(Master* master, Map* map_root) {

  Map *map = NULL;
  short map_y = SCREEN_HEIGHT/4;

  SDL_FillRect(master->stdscr, NULL, SDL_MapRGB(master->stdscr->format, 0, 0, 0));
  
  vvx_draw_text(master, CAPTION, master->stdscr->w/2, 30, 1, 3);

  vvx_draw_text(master, "Maps:", master->stdscr->w/2, SCREEN_HEIGHT/4, 1, 4);
  for (map = map_root->next; map != NULL; map = map->next)
      vvx_draw_text(master, map->name, master->stdscr->w/2, (map_y += 15), 1, 5);

  vvx_draw_text(master, "Code by Olle K", master->stdscr->w/2, SCREEN_HEIGHT-45, 1, 3);
  vvx_draw_text(master, "Images by Sofie Aid", master->stdscr->w/2, SCREEN_HEIGHT-30, 1, 3);

  SDL_Flip(master->stdscr);
}

/* Update parts of main menu */
void vvx_update_main_menu(Master* master, Map* map_root, char highmap) {

  Map *map = NULL;
  unsigned short map_y = SCREEN_HEIGHT/4;
  unsigned char mapno, longest_map_name = 0;
  SDL_Rect rect;
  
  
  /* Update Maps-part (e.g hover)*/
  if (map_root != NULL) {
    vvx_draw_text(master, "Maps:", master->stdscr->w/2, SCREEN_HEIGHT/4, 1, 4);
    for (map = map_root->next, mapno = 1; map != NULL; map = map->next, mapno++) {
      map_y += 15;
      if (strlen(map->name) > longest_map_name) longest_map_name = strlen(map->name);
      if (mapno == highmap)
        vvx_draw_text(master, map->name, master->stdscr->w/2, map_y, 1, 2);
      else
        vvx_draw_text(master, map->name, master->stdscr->w/2, map_y, 1, 5);
    }
    
    rect.x = master->stdscr->w/2 - longest_map_name*TILESIZE/2;
    rect.y = SCREEN_HEIGHT/4 - TILESIZE/2;
    rect.w = longest_map_name*TILESIZE;
    rect.h = map_y + TILESIZE*2 - SCREEN_HEIGHT/4;
    
    SDL_UpdateRect(master->stdscr, rect.x, rect.y, rect.w, rect.h);
  }
  
}

void vvx_draw_text(Master* master, const char text[], int x, int y, int is_c, int clr) {

  SDL_Color text_color = { 0, 0, 0, 0};
  const SDL_Color bg_color = { 0, 0, 0, 0 };
  SDL_Surface *text_surface = NULL;
  SDL_Rect text_rect;

  switch(clr) {
    case 1: /* Blue */
      text_color.b = 255;
      break;
      
    case 2: /* Green */
      text_color.g = 255;
      break;

    case 3: /* Orange */
      text_color.r = 255;
      text_color.g = 127;
      break;

    case 4: /* Pink */
      text_color.r = 255;
      text_color.g = 192;
      text_color.b = 203;
      break;

    case 5: /* Grey */
      text_color.r = 128;
      text_color.g = 128;
      text_color.b = 128;
      break;

    case 6: /* Red */
      text_color.r = 255;
      break;

    case 7: /* Olive */
      text_color.r = 128;
      text_color.g = 128;
      break;

    case 8: /* Cyan */
      text_color.g = 255;
      text_color.b = 255;
      break;

    default:break;
  }
    
  text_surface = TTF_RenderText_Shaded(master->font, text, text_color, bg_color);

  if (is_c) text_rect.x = x - text_surface->w/2;
  else text_rect.x = x;
  text_rect.y = y;

  SDL_BlitSurface(text_surface, NULL, master->stdscr, &text_rect);
  SDL_FreeSurface(text_surface);
  
}

void vvx_draw_all_caps(Master* master) {

  Cap *cap_ptr = master->cap_root->next;
  Player *player_ptr = master->player_root->next;
  Cap *cap_list_ptr = NULL;

  unsigned short scoreboard_x = master->map_offset_x;
  unsigned short cap_counter = 0;
  char cap_counter_str[5];

  SDL_Rect source, target;
  source.w = TILESIZE; source.h = TILESIZE;

  SDL_FillRect(master->stdscr, NULL, SDL_MapRGB(master->stdscr->format, 0, 0, 0));

/* Draw all free caps: */
  while (cap_ptr != NULL) {
    source.x = cap_ptr->color*TILESIZE;
    source.y = 0;
    
    target.x = cap_ptr->x*TILESIZE + master->map_offset_x;
    target.y = cap_ptr->y*TILESIZE + master->map_offset_y;
    SDL_BlitSurface(master->imgscr, &source, master->stdscr, &target);
    
    cap_ptr = cap_ptr->next;
  }

  /* Draw all free players: */
  while (player_ptr != NULL) {
    cap_list_ptr = player_ptr->cap_list->next;

    source.w = TILESIZE;
    source.x = player_ptr->color*TILESIZE;
    source.y = player_ptr->symbol*TILESIZE + SYMBOL_OFFSET;

    target.x = scoreboard_x; scoreboard_x += TILESIZE;
    target.y = master->map_height*TILESIZE + master->map_offset_y*2 + player_ptr->symbol*20;
    SDL_BlitSurface(master->imgscr, &source, master->stdscr, &target);
    
    while (cap_list_ptr != NULL) {

      cap_counter++;
      
      source.w = TILESIZE;
      target.x = cap_list_ptr->x*TILESIZE + master->map_offset_x;
      target.y = cap_list_ptr->y*TILESIZE + master->map_offset_y;
      SDL_BlitSurface(master->imgscr, &source, master->stdscr, &target);

      source.w = 1;
      target.x = scoreboard_x; scoreboard_x += 1;
      target.y = master->map_height*TILESIZE + master->map_offset_y*2 + player_ptr->symbol*20;
      SDL_BlitSurface(master->imgscr, &source, master->stdscr, &target);
      
      cap_list_ptr = cap_list_ptr->next;
    }

    sprintf(cap_counter_str, " %d", cap_counter);
    vvx_draw_text(master, cap_counter_str, scoreboard_x, target.y, 0, player_ptr->color);

    scoreboard_x = master->map_offset_x;
    player_ptr = player_ptr->next;
  }

}
void vvx_draw_capture(Master* master) {
  
  Cap *cap_p = NULL;

  unsigned short scoreboard_x = master->map_offset_x;
  unsigned short cap_counter = 0;
  char cap_counter_str[5];

  SDL_Rect source, target;
  source.x = master->current_player->color*TILESIZE;
  source.y = SYMBOL_OFFSET + master->current_player->symbol*TILESIZE;
  source.w = TILESIZE; source.h = TILESIZE;

  target.x = scoreboard_x; scoreboard_x += 15;
  target.y = master->map_height*TILESIZE +
    master->map_offset_y*2 + master->current_player->symbol*20;
  SDL_BlitSurface(master->imgscr, &source, master->stdscr, &target);

  for (cap_p = master->current_player->cap_list->next; cap_p != NULL; cap_p = cap_p->next) {

    cap_counter++;
    
    source.w = TILESIZE;
    target.x = cap_p->x*TILESIZE + master->map_offset_x;
    target.y = cap_p->y*TILESIZE + master->map_offset_y;
    SDL_BlitSurface(master->imgscr, &source, master->stdscr, &target);

    source.w = 1;
    target.x = scoreboard_x; scoreboard_x += 1;
    target.y = master->map_height*TILESIZE + master->map_offset_y*2 +
      master->current_player->symbol*20;
    SDL_BlitSurface(master->imgscr, &source, master->stdscr, &target);
  }


  sprintf(cap_counter_str, " %d", cap_counter);
  vvx_draw_text(master, cap_counter_str, scoreboard_x, target.y, 0,
                master->current_player->color);

}

void vvx_draw_hoverlist(Master* master, int pad) {
  
  Cap *cap_p = NULL;

  SDL_Rect source, target;
  source.x = master->current_player->hover_color*TILESIZE;
  if (pad) source.y = SYMBOL_OFFSET + master->current_player->symbol*TILESIZE;
  else source.y = TILESIZE;
  source.w = TILESIZE; source.h = TILESIZE;
  target.w = TILESIZE; target.h = TILESIZE;

  if (master->current_player->hover_list->next == NULL)
    return;

  for (cap_p = master->current_player->hover_list->next; cap_p != NULL; cap_p = cap_p->next){
    target.x = cap_p->x*TILESIZE + master->map_offset_x;
    target.y = cap_p->y*TILESIZE + master->map_offset_y;

    SDL_BlitSurface(master->imgscr, &source, master->stdscr, &target);
  }
}

void vvx_hide_hoverlist(Master* master) {

  Cap *cap_p = NULL;

  SDL_Rect source, target;
  source.x = master->current_player->hover_color*TILESIZE;
  source.y = 0;
  source.w = TILESIZE; source.h = TILESIZE;
  target.w = TILESIZE; target.h = TILESIZE;

  if (master->current_player->hover_list->next == NULL)
    return;

  for (cap_p = master->current_player->hover_list->next; cap_p != NULL; cap_p = cap_p->next){
    
    target.x = cap_p->x*TILESIZE + master->map_offset_x;
    target.y = cap_p->y*TILESIZE + master->map_offset_y;

    SDL_BlitSurface(master->imgscr, &source, master->stdscr, &target);
  }
}