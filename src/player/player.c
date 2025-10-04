/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 21:11:38 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/01 21:48:24 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static int	get_tex_y(mlx_texture_t *texture, int y, t_wall_draw *wd)
{
	int	tex_y;

	tex_y = (int)((float)(y - wd->wall_start)
			/ (float)wd->wall_height * texture->height);
	if (tex_y < 0)
		tex_y = 0;
	if (tex_y >= (int)texture->height)
		tex_y = texture->height - 1;
	return (tex_y);
}

static int	get_tex_x(t_ray_hit ray_hit, mlx_texture_t *texture)
{
	int	tex_x;

	tex_x = (int)(ray_hit.wall_x * texture->width);
	if (tex_x < 0)
		tex_x = 0;
	if (tex_x >= (int)texture->width)
		tex_x = texture->width - 1;
	return (tex_x);
}

// this increment in the condition could mess things up | this note is for later
// so we don't spend hourse trying to figure out why things don't work
// screw norm
static void	draw_wall_column(t_game *game, t_wall_draw *wd,
			mlx_texture_t *texture, t_ray_hit ray_hit)
{
	int	x;
	int	y;
	int	tex_x;
	int	tex_y;

	x = wd->x_start;
	while (x++ < wd->x_end)
	{
		if (x >= 0 && x < game->width)
		{
			y = wd->wall_start;
			while (y < wd->wall_end)
			{
				if (y++ >= 0 && y < game->height)
				{
					tex_x = get_tex_x(ray_hit, texture);
					tex_y = get_tex_y(texture, y, wd);
					mlx_put_pixel(game->frame, x, y,
						get_pixel_color(texture, tex_x, tex_y));
				}
			}
		}
	}
}

void	draw_wall(t_game *game, t_ray_hit ray_hit, int ray)
{
	t_wall_draw		wd;
	mlx_texture_t	*texture;

	init_wall_params(game, ray_hit, ray, &wd);
	texture = get_wall_texture(game, ray_hit);
	if (!texture || !texture->pixels)
		return ;
	draw_wall_column(game, &wd, texture, ray_hit);
}

void	draw_player_vision(t_game *game)
{
	int			i;
	double		sangle;
	double		angle_step;
	double		current_angle;
	t_ray_hit	ray_hit;

	sangle = game->player.r - (FOV_RAD / 2.0);
	angle_step = FOV_RAD / game->rays_number;
	i = 0;
	while (i < game->rays_number)
	{
		current_angle = sangle + i * angle_step;
		ray_hit = cast_ray(game, current_angle);
		draw_wall(game, ray_hit, i);
		i++;
	}
}
