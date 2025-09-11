#include "map.h"
#include <stdio.h>

void	print_grid(t_grid *grid)
{
	printf("Map Width: %zu\n", grid->w);
    printf("Map Height: %zu\n", grid->h);
    printf("Map Blocks:\n");
    
    for (size_t y = 0; y < grid->h; y++) {
        for (size_t x = 0; x < grid->w; x++) {
            putchar(grid->raw[y * grid->w + x]);
        }
        putchar('\n');
    }
}

void print_map(t_map *map)
{
    printf("North Texture: %s\n", map->north);
    printf("South Texture: %s\n", map->south);
    printf("West Texture: %s\n", map->west);
    printf("East Texture: %s\n", map->east);
    printf("Floor Color: %6X\n", map->floor_color);
    printf("Ceiling Color: %6X\n", map->ceiling_color);
    printf("first_map_line: %zu\n", map->first_map_line);
	print_grid(&map->grid);
}
