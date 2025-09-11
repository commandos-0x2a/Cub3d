/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:43:36 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/09 17:24:37 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"
#include <stdio.h>

// simple player control
void	player_control(void *param)
{
	t_game		*game;
	t_player	*player;
	t_vector	vec;

	game = param;
	player = &game->player;

	vec = (t_vector){0, 0};
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
	if (mlx_is_key_down(game->mlx, MLX_KEY_W)) // front
		vec.y -= 1;
	if (mlx_is_key_down(game->mlx, MLX_KEY_S)) // back
		vec.y += 1;
	if (mlx_is_key_down(game->mlx, MLX_KEY_A)) // left
		vec.x -= 1;
	if (mlx_is_key_down(game->mlx, MLX_KEY_D)) // right
		vec.x += 1;
	// The math function uses radians (PI for 180d) instead of normal degress
	if (mlx_is_key_down(game->mlx, MLX_KEY_Q))
		player->r -= 0.014;
	if (mlx_is_key_down(game->mlx, MLX_KEY_E))
		player->r += 0.014;
	vec.x *= player->speed * game->time_delta;
	vec.y *= player->speed * game->time_delta;
	player->pos.x += vec.x;
	player->pos.y += vec.y;
}
