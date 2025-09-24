/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_player.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 01:25:31 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/02/02 10:47:03 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "player.h"
#include "game_schema.h"

t_point		valid_move(t_game_schema *gs, t_player *player, \
						t_point new_location);

static void	set_new_location(t_game_schema *gs, t_player *player)
{
	t_point	new_location;

	new_location = ((t_object *)player)->relative_location;
	if (player->direction == FRONT)
		new_location.y += player->speed;
	else if (player->direction == BACK)
		new_location.y -= player->speed;
	else if (player->direction == LEFT)
		new_location.x -= player->speed;
	else if (player->direction == RIGHT)
		new_location.x += player->speed;
	new_location = valid_move(gs, player, new_location);
	player->spr.obj.relative_location = new_location;
}

void	update_player(t_player *player)
{
	t_point			old_point;
	t_game_schema	*gs;

	if (player->move_lock)
		return ;
	gs = (t_game_schema *)player->gs;
	player->spr.run_animate = 0;
	if (player->movement == SLASH_128)
		((t_object *)player)->center_point = (t_point){64, 32 + 56};
	else
		((t_object *)player)->center_point = (t_point){32, 56};
	if (player->movement == WALK && player->is_walk)
	{
		old_point = player->spr.obj.relative_location;
		set_new_location(gs, player);
		if (is_new_movement(old_point, player->spr.obj.relative_location))
			player->nb_movement += 1;
		player->spr.run_animate = 1;
	}
	if (player->movement == SLASH_128 \
			&& player->touch_component \
			&& is_object_type(player->touch_component, "tree") \
			&& ((t_tree *)player->touch_component)->status != 2)
		player->spr.run_animate = 1;
}
