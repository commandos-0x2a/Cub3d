/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_render.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:20:22 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/12 08:12:47 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"
#include "minimap.h"
#include "libft.h"

void	setup_matrix(t_minimap *minimap, float mat[3][3])
{
	mat[0][0] = minimap->ref_x * cosf(minimap->r);
	mat[0][1] = -minimap->ref_y * sinf(minimap->r);
	mat[0][2] = minimap->x;
	mat[1][0] = minimap->ref_x * sinf(minimap->r);
	mat[1][1] = minimap->ref_y * cosf(minimap->r);
	mat[1][2] = minimap->y;
	mat[2][0] = 0;
	mat[2][1] = 0;
	mat[2][2] = 1;
}

void	setup_minimap(t_minimap *minimap, t_player *player, t_map *map)
{
	minimap->x = 150 + 10;
	minimap->y = 150 + 10;
	minimap->r = player->r + PI / 2;
	minimap->ref_x = 1;
	minimap->ref_y = 1;
	minimap->width = 300;
	minimap->height = 300;
	(void)map;
	setup_matrix(minimap, minimap->mat);
}

void	transform_point(t_point *result, float mat[3][3], t_point point)
{
	result->x = mat[0][0] * point.x + mat[0][1] * point.y + mat[0][2];
	result->y = mat[1][0] * point.x + mat[1][1] * point.y + mat[1][2];
}

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

	// map_x = grid->w - map_x;
	// map_y = grid->h - map_y;
	if (!is_in_box(map_x, map_y, grid->w, grid->h))
		color = 0x000000ff;
	else if (grid->raw[map_y * grid->w + map_x] == '1')
		color = 0x00ff00ff;
	else if (grid->raw[map_y * grid->w + map_x] == '0')
		color = 0x000000ff;
	else
		color = 0x111111ff;
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
			if (powf(x - minimap->width / 2, 2) + powf(y - minimap->height / 2, 2) 
				> minimap->width / 2 * minimap->height / 2)
				continue;
			point.x = x - minimap->width / 2;
			point.y = y - minimap->height / 2;
			transform_point(&point, minimap->mat, point);	
			if (!is_in_box(point.x, point.y, frame->width, frame->height))
				continue;
			map_x = player->pos.x - (x - minimap->width / 2) / DRAWING_SCALE;
			map_y = player->pos.y - (y - minimap->height / 2) / DRAWING_SCALE;
			color = get_map_color(grid, map_x, map_y);			
			mlx_put_pixel(frame, point.x, point.y, color);
		}
	}
	draw_circle(frame, (t_point){minimap->x, minimap->y}, 16, 0xff0000ff);
}

void	minimap_render(t_game *game)
{
	t_minimap	minimap;

	ft_bzero(&minimap, sizeof(minimap));
	setup_minimap(&minimap, &game->player, game->map);
	draw_minimap(game->frame, &minimap, &game->player, &game->map->grid);
	
}
