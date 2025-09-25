/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 18:35:26 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/25 07:06:26 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_schema.h"
#include "components.h"
#include "config.h"

// void	player_walk(void *)
// {
	
// 	if (mlx_is_key_down(mlx, MLX_KEY_UP))
// 		player->direction = BACK;
// 	else if (mlx_is_key_down(mlx, MLX_KEY_DOWN))
// 		player->direction = FRONT;
// 	else if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))
// 		player->direction = RIGHT;
// 	else if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
// 		player->direction = LEFT;
// 	else
// 		return ;
// 	if (player->movement != WALK)
// 	{
// 		player->movement = WALK;
// 		((t_sprites *)player)->index = 0;
// 		((t_sprites *)player)->max_index = 9;
// 		player->spr.delay = 1;
// 	}
// 	player->is_walk = 1;
// 	return ;
// }

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
