/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_render.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:42:36 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/13 18:59:02 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minimap.h"
#include "game.h"

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
	else
		color = 0xffffffff;
	return (color);
}

void	draw_minimap(mlx_image_t *frame, t_minimap *minimap,
		t_player *player, t_grid *grid)
{
	t_point		point;
	int			map_x;
	int			map_y;
	uint32_t	color;

	for (float y = 0; y < (float)minimap->height; y += 0.5f)
	{
		for (float x = 0; x < (float)minimap->width; x += 0.5f)
		{
			point.x = x - minimap->width / 2;
			point.y = y - minimap->height / 2;
			transform_point(&point, minimap->mat, point);	
			if (!is_in_box(point.x, point.y, frame->width, frame->height))
				continue;
			map_x = player->pos.x + (x - minimap->width / 2) / DRAWING_SCALE;
			map_y = player->pos.y + (y - minimap->height / 2) / DRAWING_SCALE;
			color = get_map_color(grid, map_x, map_y);			
			mlx_put_pixel(frame, point.x, point.y, color);
		}
	}
	draw_circle(frame, (t_point){minimap->x, minimap->y}, 20, 0xff0000ff);
}
