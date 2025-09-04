#include "map.h"
#include <stdio.h>

void print_map(t_map *map)
{
    printf("North Texture: %s\n", map->north);
    printf("South Texture: %s\n", map->south);
    printf("West Texture: %s\n", map->west);
    printf("East Texture: %s\n", map->east);
    printf("Floor Color: %d\n", map->floor_color);
    printf("Ceiling Color: %d\n", map->ceiling_color);
    printf("Map Width: %zu\n", map->w);
    printf("Map Height: %zu\n", map->h);
    printf("Map Blocks:\n");
    
    // for (size_t y = 0; y < map->h; y++) {
    //     for (size_t x = 0; x < map->w; x++) {
    //         putchar(map->blocks[y * map->w + x]);
    //     }
    //     putchar('\n');
    // }
}

int main()
{
    t_map   *map;

    map = read_map("test.cub");
    if (!map)
        return (1);
    print_map(map);
    return (0);
}