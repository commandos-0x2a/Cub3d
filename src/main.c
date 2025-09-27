/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 09:28:39 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/27 08:12:00 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
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
	game.player.speed = 4;
	game.player.r = 0;
	game.player.pos.x = 4;
	game.player.pos.y = 4;
	// debug
	game.debug.sec = 0;
	game.debug.fps = 0;

	if (load_textures(&game) != 0)
	{
		printf("Error load textures\n");
		return (1);
	}

	mlx_loop_hook(game.mlx, render, &game);
	mlx_loop_hook(game.mlx, player_control, &game);
	mlx_resize_hook(game.mlx, resize_hook, &game);

	mlx_loop(game.mlx);
	mlx_terminate(game.mlx);
	return (0);
}
