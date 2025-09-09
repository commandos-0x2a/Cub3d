#include "map.h"
#include <stdio.h>
#include <MLX42/MLX42.h>
#include "game.h"

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
	t_game	game;

	if (argc != 2)
	{
		printf("%s map_file\n", argv[0]);
		return (1);
	}
	game.map = read_map(argv[1]);
	if (!game.map)
		return (1);
	if (!validate_map(game.map))
		return (1);
	print_map(game.map);

	if (!(game.mlx = mlx_init(WIDTH, HEIGHT, "MLX42", true)))
	{
		puts(mlx_strerror(mlx_errno));
		return(1);
	}
	
	if (!(game.frame = mlx_new_image(game.mlx, WIDTH, HEIGHT)))
	{
		mlx_close_window(game.mlx);
		puts(mlx_strerror(mlx_errno));
		return(1);
	}
	if (mlx_image_to_window(game.mlx, game.frame, 0, 0) == -1)
	{
		mlx_close_window(game.mlx);
		puts(mlx_strerror(mlx_errno));
		return(1);
	}

	game.last_render = mlx_get_time();

	// player
	game.player.speed = 1;
	game.player.r = 0;

	mlx_loop_hook(game.mlx, player_control, &game);
	mlx_loop_hook(game.mlx, render, &game);

	mlx_loop(game.mlx);
	// free map
	mlx_terminate(game.mlx);
	return (0);
}