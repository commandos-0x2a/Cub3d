/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:19:05 by hassende          #+#    #+#             */
/*   Updated: 2025/10/09 10:03:06 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

#define MINIMAP_SIZE_FIXED 150

static void	set_tile_properties(t_tile_map *tm, t_game *game, int tile_px_size)
{
	tm->tile = game->map->grid.raw[tm->map_y * tm->map_w + tm->map_x];
	if (tm->tile == '1')
		tm->color = 0xFFFFFFFF;
	else
		tm->color = 0x444444FF;
	tm->tile_start_x = (tm->dx + VIEW_RADIUS) * tile_px_size;
	tm->tile_start_y = (tm->dy + VIEW_RADIUS) * tile_px_size;
}

static int	is_valid_screen_pos(int screen_x, int screen_y, int tile_px_size)
{
	return (screen_x >= MINIMAP_X
		&& screen_x < MINIMAP_X + (VIEW_RADIUS * 2 + 1) * tile_px_size
		&& screen_y >= MINIMAP_Y
		&& screen_y < MINIMAP_Y + (VIEW_RADIUS * 2 + 1) * tile_px_size);
}

static void	draw_single_tile_pixels(t_game *game, t_tile_map *tm,
		int tile_px_size)
{
	tm->py = 0;
	while (tm->py < tile_px_size)
	{
		tm->px = 0;
		while (tm->px < tile_px_size)
		{
			tm->screen_x = MINIMAP_X + tm->tile_start_x + tm->px;
			tm->screen_y = MINIMAP_Y + tm->tile_start_y + tm->py;
			if (is_valid_screen_pos(tm->screen_x, tm->screen_y, tile_px_size))
				mlx_put_pixel(game->frame, tm->screen_x,
					tm->screen_y, tm->color);
			tm->px++;
		}
		tm->py++;
	}
}

static void	process_map_tile(t_game *game, t_tile_map *tm, int tile_px_size)
{
	if (tm->map_x >= 0 && tm->map_x < tm->map_w
		&& tm->map_y >= 0 && tm->map_y < tm->map_h)
	{
		set_tile_properties(tm, game, tile_px_size);
		draw_single_tile_pixels(game, tm, tile_px_size);
	}
}

void	draw_local_map_tiles(t_game *game, int player_tile_x,
		int player_tile_y, int tile_px_size)
{
	t_tile_map	tm;

	tm.map_w = game->map->grid.w;
	tm.map_h = game->map->grid.h;
	tm.dy = -VIEW_RADIUS;
	while (tm.dy <= VIEW_RADIUS)
	{
		tm.dx = -VIEW_RADIUS;
		while (tm.dx <= VIEW_RADIUS)
		{
			tm.map_x = player_tile_x + tm.dx;
			tm.map_y = player_tile_y + tm.dy;
			process_map_tile(game, &tm, tile_px_size);
			tm.dx++;
		}
		tm.dy++;
	}
}
