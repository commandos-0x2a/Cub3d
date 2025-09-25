/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 18:35:26 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/25 10:43:00 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_schema.h"
#include "components.h"
#include "config.h"

void	player_walk(t_player *player, void *mlx)
{
	if (mlx_is_key_down(mlx, MLX_KEY_KP_8))
	{
		player->direction = BACK;
		player->movement = WALK;
	}
	else if (mlx_is_key_down(mlx, MLX_KEY_KP_5))
	{
		player->direction = FRONT;
		player->movement = WALK;
	}
	else if (mlx_is_key_down(mlx, MLX_KEY_KP_6))
	{
		player->direction = RIGHT;
		player->movement = WALK;
	}
	else if (mlx_is_key_down(mlx, MLX_KEY_KP_4))
	{
		player->direction = LEFT;
		player->movement = WALK;
	}
	else if (mlx_is_key_down(mlx, MLX_KEY_SPACE))
	{
		player->is_walk = 0;
		player_slash(player);
		// if (player->logs_count >= nb_collect)
		// 	if (is_surround_boat(so))
		// 		ride_boat(so);
		return ;
	}
	else
	{
		player->is_walk = 0;
		player->movement = WALK;
		player->spr.max_index = 9;
		player->spr.index = 0;
		return ;
	}
	// printf("move: %d | dir: %d\n", player->movement, player->direction);
	if (player->movement != WALK)
	{
		player->movement = WALK;
		((t_sprites *)player)->index = 0;
		((t_sprites *)player)->max_index = 9;
		player->spr.delay = 1;
	}
	player->is_walk = 1;
	return ;
}

void	player_slash(t_player *player)
{
	if (player->movement == SLASH_128)
		return ;
	if (player->touch_component \
		&& is_object_type(player->touch_component, "tree"))
	{
		if (((t_tree *)player->touch_component)->status != 2)
		{
			player->movement = SLASH_128;
			((t_sprites *)player)->index = 0;
			((t_sprites *)player)->max_index = 6;
			player->spr.run_animate = 1;
		}
	}
}
