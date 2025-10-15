/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_render.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:42:36 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/15 16:20:26 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minimap.h"
#include "game.h"
#include "libft.h"

void	draw_circle(mlx_image_t *frame, t_point p, int radius, uint32_t color)
{
	int	x;
	int	y;
	int	draw_x;
	int	draw_y;

	y = 0;
	while (y < radius * 2)
	{
		x = 0;
		while (x < radius * 2)
		{
			draw_x = p.x + x - radius;
			draw_y = p.y + y - radius;
			if (is_in_circle(x - radius, y - radius, radius)
				&& is_in_box(draw_x, draw_y, frame->width, frame->height))
				mlx_put_pixel(frame, draw_x, draw_y, color);
			x++;
		}
		y++;
	}
}

uint32_t	get_map_color(t_grid *grid, int map_x, int map_y)
{
	uint32_t	color;

	if (!is_in_box(map_x, map_y, grid->w, grid->h))
		color = 0x000000ff;
	else if (grid->raw[map_y * grid->w + map_x] == '1')
		color = 0x00ff00ff;
	else if (grid->raw[map_y * grid->w + map_x] == '0')
		color = 0x000000ff;
	else if (grid->raw[map_y * grid->w + map_x] == 'O')
		color = 0xaa5500ff;
	else if (ft_strchr("NSEW", grid->raw[map_y * grid->w + map_x]))
		color = 0x000000ff;
	else if (grid->raw[map_y * grid->w + map_x] == 'D')
		color = 0xff00ffff;
	else
		color = 0xffffffff;
	return (color);
}

t_point	get_draw_point(t_minimap *minimap, float x, float y)
{
	t_point	draw_point;
	t_point	center_point;

	center_point.x = x - minimap->width / 2;
	center_point.y = y - minimap->height / 2;
	draw_point = transform_point(minimap->mat, center_point);
	return (draw_point);
}

void	draw_minimap(mlx_image_t *frame, t_minimap *minimap,
		t_player *player, t_grid *grid)
{
	t_point		draw_p;
	int			map_x;
	int			map_y;
	float		x;
	float		y;

	y = 0;
	while (y < (float)minimap->height)
	{
		x = 0;
		while (x < (float)minimap->width)
		{
			draw_p = get_draw_point(minimap, x, y);
			if (!is_in_box(draw_p.x, draw_p.y, frame->width, frame->height))
				continue ;
			map_x = player->pos.x + (x - minimap->width / 2) / DRAWING_SCALE;
			map_y = player->pos.y + (y - minimap->height / 2) / DRAWING_SCALE;
			mlx_put_pixel(frame, draw_p.x, draw_p.y,
				get_map_color(grid, map_x, map_y));
			x += 0.5f;
		}
		y += 0.5f;
	}
	draw_circle(frame, (t_point){minimap->x, minimap->y}, 20, 0xff0000ff);
}
