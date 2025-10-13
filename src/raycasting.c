/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 21:11:38 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/13 13:51:31 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static void	set_step_and_side_dist(t_ray_cast *rc)
{
	rc->step_x = (int)copysignf(1.f, rc->dir_x);
	rc->step_y = (int)copysignf(1.f, rc->dir_y);
	if (rc->dir_x < 0)
		rc->side_dist_x = (rc->px - rc->map_x) * rc->delta_dist_x;
	else
		rc->side_dist_x = (rc->map_x + 1.0 - rc->px) * rc->delta_dist_x;
	if (rc->dir_y < 0)
		rc->side_dist_y = (rc->py - rc->map_y) * rc->delta_dist_y;
	else
		rc->side_dist_y = (rc->map_y + 1.0 - rc->py) * rc->delta_dist_y;
}

static int	check_wall_hit(t_map *map, t_ray_cast *rc, t_ray_hit *ray)
{
	if (!is_in_box(rc->map_x, rc->map_y, map->grid.w, map->grid.h))
		return (1);
	if (map->grid.raw[rc->map_y * map->grid.w + rc->map_x] == '1')
		return (1);
	if (map->grid.raw[rc->map_y * map->grid.w + rc->map_x] == 'D')
	{
		ray->tex_i = 4;
		return (1);
	}
	return (0);
}

static void	perform_dda(t_map *map, t_ray_cast *rc, t_ray_hit *ray)
{
	int	hit;

	hit = 0;
	while (!hit)
	{
		if (rc->side_dist_x < rc->side_dist_y)
		{
			rc->side_dist_x += rc->delta_dist_x;
			rc->map_x += rc->step_x;
			rc->side = 0;
		}
		else
		{
			rc->side_dist_y += rc->delta_dist_y;
			rc->map_y += rc->step_y;
			rc->side = 1;
		}
		hit = check_wall_hit(map, rc, ray);
	}
}

static void	calculate_distance_and_texture(t_ray_cast *rc, t_ray_hit *ray)
{
	if (rc->side == 0)
	{
		ray->distance = (rc->map_x - rc->px + (1 - rc->step_x) / 2) / rc->dir_x;
		ray->wall_x = rc->py + ray->distance * rc->dir_y;
	}
	else
	{
		ray->distance = (rc->map_y - rc->py + (1 - rc->step_y) / 2) / rc->dir_y;
		ray->wall_x = rc->px + ray->distance * rc->dir_x;
	}
	ray->wall_x -= floor(ray->wall_x);
	if (ray->wall_x < 0)
		ray->wall_x += 1.0f;
	if (ray->tex_i == 4)
		return ;
	if (rc->side == 0 && rc->step_x == 1)
		ray->tex_i = WALL_EAST;
	else if (rc->side == 0 && rc->step_x == -1)
		ray->tex_i = WALL_WEST;
	else if (rc->side == 1 && rc->step_y == 1)
		ray->tex_i = WALL_SOUTH;
	else
		ray->tex_i = WALL_NORTH;
}

t_ray_hit	cast_ray(t_game *game, double angle)
{
	t_ray_hit	ray;
	t_ray_cast	rc;

	ray.tex_i = 0;
	rc.px = game->player.pos.x;
	rc.py = game->player.pos.y;
	rc.dir_x = cos(angle);
	rc.dir_y = sin(angle);
	rc.map_x = (int)rc.px;
	rc.map_y = (int)rc.py;
	rc.delta_dist_x = fabsf(1.0f / rc.dir_x);
	rc.delta_dist_y = fabsf(1.0f / rc.dir_y);
	set_step_and_side_dist(&rc);
	perform_dda(game->map, &rc, &ray);
	calculate_distance_and_texture(&rc, &ray);
	return (ray);
}
