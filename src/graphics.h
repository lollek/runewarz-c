#ifndef RUNEWARZ_GRAPHICS_H
#define RUNEWARZ_GRAPHICS_H

#define TILES_PATH "img/tiles.png"
#define FONT_PATH "img/freemonobold.ttf"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_BPP 24
#define TILESIZE 15
#define SYMBOL_OFFSET 15

#define CAPTION "RuneWarz v0.2.1"

/**
 * Start up SDL and its subsystems, then load all needed images from disk.
 * If any errors occur, they will be printed to stderr.
 *
 * Returns:
 *  0 on success or non-fatal error
 *  1 on fatal error
 */
int graphics_init();

/**
 * Stop SDL and its subsystems. Also free all allocated resources
 */
void graphics_exit();

/**
 * Paint the whole screen black
 *
 * Returns:
 *  0 on success
 *  1 on error
 */
int graphics_paint_screen_black();

/**
 * Print the given text centered on the screen
 *
 * Parameters:
 *  text: Text to print
 *  height: position of the text
 *
 * Returns:
 *  0 on success
 *  1 on error
 */
int graphics_print_text(const char *text, int height);

/**
 * Show all updates done to the screen
 *
 * Returns:
 *  0 on success
 *  1 on error
 */
int graphics_draw_updates();

#endif /* RUNEWARZ_GRAPHICS_H */
