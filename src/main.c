#include <stdio.h>

#include "graphics.h"
#include "menu.h"

int main(void)
  {
    if (graphics_init() != 0)
      {
        return 1;
      }

    main_menu();

    graphics_exit();
    return 0;
  }
