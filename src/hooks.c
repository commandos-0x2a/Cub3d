/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:51:37 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/27 14:28:06 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

void resize_hook(int32_t width, int32_t height, void *param)
{
	t_game	*game;

	game = param;
	game->rays_number = width;
	game->width = width;
	game->height = height;
}

void    game_hooks(t_game *game)
{
    mlx_loop_hook(game->mlx, render, game);
	mlx_loop_hook(game->mlx, player_control, game);
	mlx_resize_hook(game->mlx, resize_hook, game);
}
