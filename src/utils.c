/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 09:54:04 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/19 16:16:32 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"
#include <math.h>

int	get_tex_x(t_game *game, t_ray_hit *ray_hit)
{
	int	tex_x;

	if (ray_hit->tex_i == WALL_WEST || ray_hit->tex_i == WALL_SOUTH)
		tex_x = (int)((1.0f - ray_hit->wall_x)
				* game->texture[ray_hit->tex_i]->width);
	else
		tex_x = (int)(ray_hit->wall_x * game->texture[ray_hit->tex_i]->width);
	tex_x = iclamp(tex_x, 0, game->texture[ray_hit->tex_i]->width - 1);
	return (tex_x);
}

int	iclamp(int value, int min, int max)
{
	return ((int)fmax(min, fmin(value, max)));
}

float	fclamp(float value, float min, float max)
{
	return (fmaxf(min, fminf(value, max)));
}

int	is_in_box(int x, int y, int w, int h)
{
	return (x >= 0 && x < w && y >= 0 && y < h);
}

int	is_in_circle(float x, float y, float radius)
{
	return (powf(x, 2) + powf(y, 2) <= radius * 2);
}
