/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_hook.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 00:16:23 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/02/02 01:26:35 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "player.h"
#include "game_schema.h"

void	aminate_player(t_player *player)
{
	player->clip.y = 0;
	if (player->logs_count >= 9)
		player->clip.y = 2 * 20 * 64;
	else if (player->logs_count >= 3)
		player->clip.y = 1 * 20 * 64;
	player->clip.y += player->movement * 4 * 64;
	if (player->movement == SLASH_128)
	{
		player->clip.x = player->spr.index * 128;
		player->clip.y += player->direction * 128;
		player->clip.width = 128;
		player->clip.height = 128;
	}
	else
	{
		player->clip.x = player->spr.index * 64;
		player->clip.y += player->direction * 64;
		player->clip.width = 64;
		player->clip.height = 64;
	}
}

void	end_move_player(t_player *player)
{
	t_tree	*t;

	t = (t_tree *)player->touch_component;
	if (player->movement == SLASH_128)
		if (t && ft_strncmp((char *)t, "tree", NAME_SIZE) == 0)
			if (damage_tree(t, 50) == 1)
				player->logs_count++;
}
