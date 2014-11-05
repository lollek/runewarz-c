#include "map.h"

#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

static char **maps = NULL;
static int num_maps = 0;

int
map_init()
  {
    DIR *dir = opendir("maps");
    if (dir == NULL)
      {
        fprintf(stderr, "Failed to open map-folder: %s\n", strerror(errno));
        return 1;
      }

    struct dirent *dir_r;
    while ((dir_r = readdir(dir)) != NULL)
      {
        if (strcmp(".", dir_r->d_name) && strcmp("..", dir_r->d_name))
          {
            ++num_maps;
          }
      }
    maps = malloc(num_maps * sizeof *maps);

    rewinddir(dir);
    int current_map = 0;
    while ((dir_r = readdir(dir)) != NULL)
      {
        if (strcmp(".", dir_r->d_name) && strcmp("..", dir_r->d_name))
          {
            maps[current_map] = malloc(strlen(dir_r->d_name) +1);
            strcpy(maps[current_map], dir_r->d_name);
            ++current_map;
          }
      }

    return 0;
  }

void
map_exit()
  {
    for (int i = 0; i < num_maps; ++i)
      {
        free(maps[i]);
      }
    num_maps = 0;

    free(maps);
    maps = NULL;
  }

int
map_num_maps()
  {
    return num_maps;
  }

const char *
map_get_map_name(int index)
  {
    return maps[index];
  }
