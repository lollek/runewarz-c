#include "vvk_graphics.h"

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

int vvk_make_main_menu_surface(SDL_Surface** img_mmenu,
                               TTF_Font** font20, TTF_Font** font15,
                               const char VERSION[]) {

  SDL_Surface *bg_img = NULL;
  const SDL_Color color_red = { 123, 0, 0, 0 };
  const SDL_Color color_red2 = { 80, 0, 0, 0 };

    
  /* Load Background Image:*/
  bg_img = IMG_Load("img/BG_IMG2.png");
  if (bg_img == NULL) {
    fprintf(stderr, "Unable to load img/BG_IMG2.png\n");
    return 1;
  }

  /* Draw all the nicey text: */
  /* 1. Title: */
  vvk_render_text_centered(&bg_img, font20, "Ath Cliath", color_red, 0, -150);
  
  /* 2. Center Box: */
  vvk_render_box_centered(&bg_img, 0, 0, 200, 200);
  
  vvk_render_text_centered(&bg_img, font15, "(N) New Game", color_red2, 0, -80);
  vvk_render_text_centered(&bg_img, font15, "(L) Load Game", color_red2, 0, -50);
  vvk_render_text_centered(&bg_img, font15, "(G) Graveyard", color_red2, 0, 0);
  vvk_render_text_centered(&bg_img, font15, "(Q) Quit", color_red2, 0, 80);

  /* 3. Bottom Box: */
  vvk_render_box_centered(&bg_img, 0, 300, 200, 70);

  vvk_render_text_centered(&bg_img, font15, VERSION, color_red2, 0, 280);
  vvk_render_text_centered(&bg_img, font15, "A C Roguelike", color_red2, 0, 300);
  vvk_render_text_centered(&bg_img, font15, "By Olle Kvarnstrom", color_red2, 0, 320);
  
  /* Optimise surface for quick blit: */
  *img_mmenu = SDL_DisplayFormat(bg_img);

  /* Clean up: */
  SDL_FreeSurface(bg_img);
  
  return 0;
}
