/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:43:36 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/15 14:55:12 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"
#include <stdio.h>
#include <math.h>

// simple player control
void	player_control(void *param)
{
	t_game		*game;
	t_player	*player;
	t_vector	vec;
	int			mapx;
	int			mapy;

	game = param;
	player = &game->player;

	vec = (t_vector){0, 0};
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
	if (mlx_is_key_down(game->mlx, MLX_KEY_W)) // front
	{
		// (cosθ,sinθ)
		vec.x += cosf(player->r);
		vec.y += sinf(player->r);
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_S)) // back
	{
		// (−cosθ,−sinθ)
		vec.x -= cosf(player->r);
		vec.y -= sinf(player->r);
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_A)) // left
	{
		// (sin θ,− cos θ)
		vec.x += sinf(player->r);
		vec.y -= cosf(player->r);
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_D)) // right
	{
		// (− sin θ, cos θ)
		vec.x -= sinf(player->r);
		vec.y += cosf(player->r);
	}
	// The math function uses radians (PI for 180d) instead of normal degress
	if (mlx_is_key_down(game->mlx, MLX_KEY_Q))
		player->r -= 0.054;
	if (mlx_is_key_down(game->mlx, MLX_KEY_E))
		player->r += 0.054;
	// printf("(%.2f, %.2f)\n", vec.x, vec.y);
	vec.x *= player->speed * game->time_delta;
	vec.y *= player->speed * game->time_delta;
	player->pos.x += vec.x;
	player->pos.y += vec.y;
	mapx = (int)player->pos.x;
	mapy = (int)player->pos.y;
	if (game->map->grid.raw[mapy * game->map->grid.w + mapx] == '1')
	{
		player->pos.x -= vec.x;
		player->pos.y -= vec.y;
	}
}
