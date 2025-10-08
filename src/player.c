/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 21:11:38 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/08 20:27:29 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

static uint32_t	get_pixel_color(mlx_texture_t *texture, int tex_x, int tex_y)
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
