#ifndef RUNEWARZ_MAP_H
#define RUNEWARZ_MAP_H

/**
 * Counts the maps in the maps-folder, and stores their names
 *
 * Returns:
 *  0 on success
 *  1 on error
 */
int map_init();

/**
 * Frees the stores map-data
 */
void map_exit();

/**
 * Returns the number of maps currently loaded
 */
int map_num_maps();

/**
 * Returns the name of the map of the given index
 *
 * Parameters:
 *  index: Needs to be 0 <= x < map_num_maps()
 */
const char *map_get_map_name(int index);

#endif /* RUNEWARZ_MAP_H */
