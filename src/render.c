/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 20:20:25 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/01 21:48:28 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"
#include <stdio.h>

void	render_floor_ceiling(mlx_image_t *frame,
	uint32_t floor_color, uint32_t ceiling_color)
{
	size_t	y;
	size_t	x;

	y = 0;
	while (y < frame->height)
	{
		x = 0;
		while (x < frame->width)
		{
			if (y < frame->height / 2)
				mlx_put_pixel(frame, x, y, ceiling_color);
			else
				mlx_put_pixel(frame, x, y, floor_color);
			x++;
		}
		y++;
	}
}

void	animate(t_game *game, double now)
{
	size_t		i;
	t_animate	*anim;

	i = 0;
	while (i < game->nb_animate)
	{
		anim = (void *)game->animates[i];
		if (anim->next_frame)
		{
			if (now - anim->last_animate > anim->frame_duration)
			{
				anim->next_frame(anim, anim->ctx);
				anim->last_animate = now;
			}
		}
		i++;
	}
}

void	render(void *param)
{
	t_game	*game;
	double	now;

	game = param;
	now = mlx_get_time();
	animate(game, now);
	game->time_delta = now - game->last_render;
	game->last_render = now;
	game->debug.sec += game->time_delta;
	game->debug.fps++;
	if (game->debug.sec >= 1)
	{
		game->debug.sec = 0;
		game->debug.fps = 0;
	}
	render_floor_ceiling(game->frame, game->map->floor_color,
		game->map->ceiling_color);
	draw_player_vision(game);
	update_minimap(game);
}
