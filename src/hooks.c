/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:51:37 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/04 10:19:16 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

void	resize_hook(int32_t width, int32_t height, void *param)
{
	t_game	*game;

	game = param;
	game->width = width;
	game->height = height;
	mlx_delete_image(game->mlx, game->frame);
	game->frame = mlx_new_image(game->mlx, game->width, game->height);
	if (!game->frame)
	{
		printf("Error: frame init\n");
		end_game(game, 1);
	}
	mlx_image_to_window(game->mlx, game->frame, 0, 0);
}

void	game_hooks(t_game *game)
{
	mlx_loop_hook(game->mlx, render, game);
	mlx_loop_hook(game->mlx, player_control, game);
	mlx_resize_hook(game->mlx, resize_hook, game);
}
