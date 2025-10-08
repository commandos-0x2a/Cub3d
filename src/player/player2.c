/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player2.c                                           :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 21:11:38 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/01 21:48:24 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static void	determine_wall_type(t_ray_cast *rc, t_ray_hit *ray)
{
	if (ray->is_vertical == 5)
		return ;
	if (rc->side == 0)
	{
		if (rc->step_x == 1)
			ray->is_vertical = WALL_EAST;
		else
			ray->is_vertical = WALL_WEST;
	}
	else
	{
		if (rc->step_y == 1)
			ray->is_vertical = WALL_SOUTH;
		else
			ray->is_vertical = WALL_NORTH;
	}
}

t_ray_hit	cast_ray(t_game *game, double angle)
{
	t_ray_hit	ray;
	t_ray_cast	rc;

	ray.is_vertical = 0;
	init_ray_params(game, angle, &rc);
	set_step_and_side_dist(&rc);
	perform_dda(game, &rc, &ray);
	calculate_distance_and_texture(&rc, &ray);
	determine_wall_type(&rc, &ray);
	return (ray);
}

uint32_t	get_pixel_color(mlx_texture_t *texture, int tex_x, int tex_y)
{
	uint8_t		*pixel;
	uint32_t	color;

	pixel = texture->pixels + (tex_y * texture->width + tex_x) * 4;
	color = (pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | pixel[3];
	return (color);
}

void	init_wall_params(t_game *game, t_ray_hit ray_hit, int ray,
			t_wall_draw *wd)
{
	double	angle_offset;

	angle_offset = (ray - game->rays_number / 2) * (FOV * PI / 180.0
			/ game->rays_number);
	wd->corrected_distance = ray_hit.distance * cos(angle_offset);
	if (wd->corrected_distance <= 1)
		wd->corrected_distance = 1;
	wd->wall_height = (int)(game->height * TILE_SIZE / wd->corrected_distance);
	if (wd->wall_height > game->height)
		wd->wall_height = game->height;
	if (wd->wall_height < 1)
		wd->wall_height = 1;
	wd->wall_start = (game->height - wd->wall_height) / 2;
	wd->wall_end = wd->wall_start + wd->wall_height;
	wd->x_start = (ray * game->width) / game->rays_number;
	wd->x_end = ((ray + 1) * game->width) / game->rays_number;
}

mlx_texture_t	*get_wall_texture(t_game *game, t_ray_hit ray_hit)
{
	mlx_texture_t	*texture;

	if (ray_hit.is_vertical == 5)
		texture = game->texture[4];
	else
		texture = game->texture[ray_hit.is_vertical];
	return (texture);
}
