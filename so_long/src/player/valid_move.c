/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_move.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 22:55:53 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/02/02 01:48:52 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_schema.h"
#include <math.h>

static int	collagen_detection(t_point p1, t_point p2, double a, double b)
{
	return (pow(p2.x - p1.x, 2) / (a * a) + pow(p2.y - p1.y, 2) / (b * b) <= 1);
}

int	is_detection(t_game_schema *gs, t_player *player, t_point new_location)
{
	t_point	circle_center;

	circle_center.x = (new_location.x / 128) * 128 + 64;
	circle_center.y = (new_location.y / 128) * 128 + 64;
	player->touch_component = get_children_by_loacation(&gs->components, \
															circle_center);
	if (player->touch_component)
	{
		if (is_object_type(player->touch_component, "tree"))
			if (collagen_detection(circle_center, new_location, 30, 15))
				return (1);
		if (is_object_type(player->touch_component, "fire"))
			if (collagen_detection(circle_center, new_location, 35, 25))
				return (1);
	}
	player->touch_component = NULL;
	return (0);
}

char	get_mask_value(t_game_schema *gs, t_point new_location)
{
	t_tile		*tile;
	t_point		tile_loc;
	t_cell		cell;
	int			index;

	tile_loc.x = (new_location.x / 64) * 64;
	tile_loc.y = (new_location.y / 64) * 64;
	tile = (t_tile *)get_children_by_loacation(&gs->map, tile_loc);
	if (!tile)
		return (0);
	cell.r = (new_location.y % 64) / (64 / MASK_SIZE);
	cell.c = (new_location.x % 64) / (64 / MASK_SIZE);
	index = cell.r * MASK_SIZE + cell.c;
	return (tile->mask[index]);
}

t_point	valid_move(t_game_schema *gs, t_player *player, \
						t_point new_location)
{
	t_point		current_location;

	current_location = ((t_object *)player)->relative_location;
	if (get_mask_value(gs, new_location) != '0')
		return (current_location);
	if (is_detection(gs, player, new_location))
		return (current_location);
	player->touch_component = NULL;
	return (new_location);
}
