/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 21:11:38 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/07 17:31:28 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static void	init_ray_params(t_game *game, double angle, t_ray_cast *rc)
{
	rc->px = game->player.pos.x;
	rc->py = game->player.pos.y;
	rc->dir_x = cos(angle);
	rc->dir_y = sin(angle);
	rc->map_x = (int)rc->px;
	rc->map_y = (int)rc->py;
	if (rc->dir_x == 0)
		rc->delta_dist_x = 1e30;
	else
		rc->delta_dist_x = fabs(1.0 / rc->dir_x);
	if (rc->dir_y == 0)
		rc->delta_dist_y = 1e30;
	else
		rc->delta_dist_y = fabs(1.0 / rc->dir_y);
}

static void	set_step_and_side_dist(t_ray_cast *rc)
{
	if (rc->dir_x < 0)
	{
		rc->step_x = -1;
		rc->side_dist_x = (rc->px - rc->map_x) * rc->delta_dist_x;
	}
	else
	{
		rc->step_x = 1;
		rc->side_dist_x = (rc->map_x + 1.0 - rc->px) * rc->delta_dist_x;
	}
	if (rc->dir_y < 0)
	{
		rc->step_y = -1;
		rc->side_dist_y = (rc->py - rc->map_y) * rc->delta_dist_y;
	}
	else
	{
		rc->step_y = 1;
		rc->side_dist_y = (rc->map_y + 1.0 - rc->py) * rc->delta_dist_y;
	}
}

static int	check_wall_hit(t_game *game, t_ray_cast *rc, t_ray_hit *ray)
{
	if (rc->map_x < 0 || rc->map_x >= (int)game->map->grid.w
		|| rc->map_y < 0 || rc->map_y >= (int)game->map->grid.h)
		return (1);
	if (game->map->grid.raw[rc->map_y * game->map->grid.w + rc->map_x] == '1')
		return (1);
	if (game->map->grid.raw[rc->map_y * game->map->grid.w + rc->map_x] == 'D')
	{
		ray->tex_i = 4;
		return (1);
	}
	return (0);
}

static void	perform_dda(t_game *game, t_ray_cast *rc, t_ray_hit *ray)
{
	int	hit;

	hit = 0;
	while (!hit)
	{
		if (rc->side_dist_x < rc->side_dist_y)
		{
			rc->side_dist_x += rc->delta_dist_x;
			rc->map_x += (int)rc->step_x;
			rc->side = 0;
		}
		else
		{
			rc->side_dist_y += rc->delta_dist_y;
			rc->map_y += (int)rc->step_y;
			rc->side = 1;
		}
		hit = check_wall_hit(game, rc, ray);
	}
}

static void	calculate_distance_and_texture(t_ray_cast *rc, t_ray_hit *ray)
{
	if (rc->side == 0)
		ray->distance = (rc->map_x - rc->px + (1 - rc->step_x) / 2) / rc->dir_x;
	else
		ray->distance = (rc->map_y - rc->py + (1 - rc->step_y) / 2) / rc->dir_y;
	if (rc->side == 0)
		ray->wall_x = rc->py + ray->distance * rc->dir_y;
	else
		ray->wall_x = rc->px + ray->distance * rc->dir_x;
	ray->wall_x -= floor(ray->wall_x);
	if (ray->wall_x < 0)
		ray->wall_x += 1.0f;
	
	if (ray->tex_i == 4)
		return ;
	if (rc->side == 0)
	{
		if (rc->step_x == 1)
			ray->tex_i = WALL_EAST;
		else
			ray->tex_i = WALL_WEST;
	}
	else
	{
		if (rc->step_y == 1)
			ray->tex_i = WALL_SOUTH;
		else
			ray->tex_i = WALL_NORTH;
	}
}


t_ray_hit	cast_ray(t_game *game, double angle)
{
	t_ray_hit	ray;
	t_ray_cast	rc;

	ray.tex_i = 0;
	init_ray_params(game, angle, &rc);
	set_step_and_side_dist(&rc);
	perform_dda(game, &rc, &ray);
	calculate_distance_and_texture(&rc, &ray);
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

static int	get_wall_height(t_game *game, float distance, int x)
{
	double	angle_offset;
	float	corrected_distance;
	int		wall_height;

	angle_offset = (x - game->width / 2) * (FOV * PI / 180.0
			/ game->width);
	corrected_distance = distance * cos(angle_offset);
	if (corrected_distance <= 0.1f)
		corrected_distance = 0.1f;
	wall_height = (int)(game->height * TILE_SIZE / corrected_distance);
	return (iclamp(wall_height, 0, game->height));
}

static void	draw_wall_column(t_game *game, mlx_texture_t *texture, int x, int wall_height,
			t_ray_hit ray_hit)
{
	int	y;
	int	start_y;
	int	end_y;
	int	tex_x;
	int	tex_y;

	start_y = (game->height - wall_height) / 2;
	end_y = (start_y + wall_height);
	y = start_y;
	while (y < end_y)
	{
		if (y >= 0 && y < game->height)
		{
			tex_x = (int)(ray_hit.wall_x * texture->width);
			tex_x = iclamp(tex_x, 0, texture->width - 1);
			tex_y = (int)((float)(y - start_y)
					/ (float)wall_height * texture->height);
			tex_y = iclamp(tex_y, 0, texture->height - 1);
			mlx_put_pixel(game->frame, x, y,
				get_pixel_color(texture, tex_x, tex_y));
		}
		y++;
	}
}

void	draw_wall(t_game *game, t_ray_hit ray_hit, int x)
{
	mlx_texture_t	*texture;
	int				wall_height;

	wall_height = get_wall_height(game, ray_hit.distance, x);
	if (ray_hit.tex_i < 0 || ray_hit.tex_i > 4)
		return ;
	texture = game->texture[ray_hit.tex_i];
	if (!texture || !texture->pixels)
		return ;
	draw_wall_column(game, texture, x, wall_height, ray_hit);
}

void	render_schema(t_game *game)
{
	int			x;
	double		sangle;
	double		angle_step;
	t_ray_hit	ray_hit;

	sangle = game->player.r - (FOV_RAD / 2.0);
	angle_step = FOV_RAD / game->width;
	x = 0;
	while (x < game->width)
	{
		ray_hit = cast_ray(game, sangle + x * angle_step);
		draw_wall(game, ray_hit, x);
		x++;
	}
}
