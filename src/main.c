#include "map.h"
#include <stdio.h>
#include <MLX42/MLX42.h>
#include "game.h"
#include "libft.h"

void resize_hook(int32_t width, int32_t height, void *param)
{
	t_game	*game;

	game = param;
	game->rays_number = width;
	game->width = width;
	game->height = height;
}

int main(int argc, char *argv[])
{
	t_game	game;

	if (argc != 2)
	{
		printf("%s map_file\n", argv[0]);
		return (1);
	}
	ft_bzero(&game, sizeof(game));
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
	game.rays_number = WIDTH;
	game.width = WIDTH;
	game.height = HEIGHT;

	if (!(game.frame = mlx_new_image(game.mlx, game.width, game.height)))
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
	game.player.pos.x = 4;
	game.player.pos.y = 4;
	// debug
	game.debug.sec = 0;
	game.debug.fps = 0;

	game.texture[WALL_NORTH] = mlx_load_png("./textures/test/NO.png");
    game.texture[WALL_SOUTH] = mlx_load_png("./textures/test/SO.png");
    game.texture[WALL_WEST]  = mlx_load_png("./textures/test/WE.png");
    game.texture[WALL_EAST]  = mlx_load_png("./textures/test/EA.png");

	mlx_loop_hook(game.mlx, render, &game);
	mlx_loop_hook(game.mlx, player_control, &game);
	mlx_resize_hook(game.mlx, resize_hook, &game);

	mlx_loop(game.mlx);
	// free map
	mlx_terminate(game.mlx);
	return (0);
}
