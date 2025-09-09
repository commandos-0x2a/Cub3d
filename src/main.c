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

void ft_hook(void* param)
{
	t_game* game;
	size_t	y;
	size_t	x;

	game = param;
	// printf("x: %u, y: %u\n", game->frame->width, game->frame->height);
	y = 0;
	while (y < game->frame->height)
	{
		x = 0;
		while (x < game->frame->width)
		{
			if (y < game->frame->height / 2)
				// mlx_put_pixel(game->frame, x, y, 0x0);
				mlx_put_pixel(game->frame, x, y, game->map->ceiling_color);
			else
				// mlx_put_pixel(game->frame, x, y, 0xffffffff);
				mlx_put_pixel(game->frame, x, y, game->map->floor_color);
			x++;
		}	
		y++;
	}
	render_world(game);
}

void	update_player(void *param)
{
	t_game	*game;

	game = param;

	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
	if (mlx_is_key_down(game->mlx, MLX_KEY_UP))
		// image->instances[0].y -= 5;
	if (mlx_is_key_down(game->mlx, MLX_KEY_DOWN))
		// image->instances[0].y += 5;
		;
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
		// image->instances[0].x -= 5;
		;
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
		// image->instances[0].x += 5;
		;
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

	// Gotta error check this stuff
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

	mlx_loop_hook(game.mlx, update_player, &game);
	mlx_loop_hook(game.mlx, ft_hook, &game);

	mlx_loop(game.mlx);
	// free map
	mlx_terminate(game.mlx);
	return (0);
}