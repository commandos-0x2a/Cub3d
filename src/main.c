#include "map.h"
#include <stdio.h>
#include <MLX42/MLX42.h>
#include "game.h"

int main()
{
	t_game	game;

	// ! commented for testing
	// if (argc != 2)
	// {
	// 	printf("%s map_file\n", argv[0]);
	// 	return (1);
	// }
	// game.map = read_map(argv[1]);
	// if (!game.map)
	// 	return (1);
	// if (!validate_map(game.map))
	// 	return (1);
	// print_map(game.map);

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
	game.player.speed = 10;
	game.player.r = 0;
	game.player.pos.x = 0;
	game.player.pos.y = 0;
	// debug
	game.debug.sec = 0;
	game.debug.fps = 0;


	mlx_loop_hook(game.mlx, render, &game);
	mlx_loop_hook(game.mlx, player_control, &game);

	mlx_loop(game.mlx);
	// free map
	mlx_terminate(game.mlx);
	return (0);
}
