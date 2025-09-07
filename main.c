#include "map.h"
#include <stdio.h>

void print_map(t_map *map)
{
    printf("North Texture: %s\n", map->north);
    printf("South Texture: %s\n", map->south);
    printf("West Texture: %s\n", map->west);
    printf("East Texture: %s\n", map->east);
    printf("Floor Color: %6X\n", map->floor_color);
    printf("Ceiling Color: %6X\n", map->ceiling_color);
    printf("Map Width: %zu\n", map->w);
    printf("Map Height: %zu\n", map->h);
    printf("first_map_line: %zu\n", map->first_map_line);
    printf("Map Blocks:\n");
    
    for (size_t y = 0; y < map->h; y++) {
        for (size_t x = 0; x < map->w; x++) {
            putchar(map->grid[y * map->w + x]);
        }
        putchar('\n');
    }
}

int main(int argc, char **argv)
{
    t_map   *map;

    if (argc != 2)
    {
        printf("%s map_file\n", argv[0]);
        return (1);
    }
    map = read_map(argv[1]);
    if (!map)
        return (1);
    if (!validate_map(map))
        return (1);
    print_map(map);
    return (0);
}