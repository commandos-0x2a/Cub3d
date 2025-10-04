/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 20:20:30 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/01 20:21:50 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

#define MINIMAP_SIZE_FIXED 150

static void	clear_minimap_background(t_game *game)
{
	int	i;
	int	j;

	i = 0;
	while (i < MINIMAP_SIZE_FIXED)
	{
		j = 0;
		while (j < MINIMAP_SIZE_FIXED)
		{
			mlx_put_pixel(game->frame, MINIMAP_X + i, MINIMAP_Y + j,
				0x000000FF);
			j++;
		}
		i++;
	}
}

static void	set_tile_properties(t_tileMap *tm, t_game *game, int tile_px_size)
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

static void	draw_single_tile_pixels(t_game *game, t_tileMap *tm,
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

static void	process_map_tile(t_game *game, t_tileMap *tm, int tile_px_size)
{
	if (tm->map_x >= 0 && tm->map_x < tm->map_w
		&& tm->map_y >= 0 && tm->map_y < tm->map_h)
	{
		set_tile_properties(tm, game, tile_px_size);
		draw_single_tile_pixels(game, tm, tile_px_size);
	}
}

static void	draw_local_map_tiles(t_game *game, int player_tile_x,
		int player_tile_y, int tile_px_size)
{
	t_tileMap	tm;

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

void	draw_player_vision_on_minimap(t_game *game, int center_px,
			t_DDAmap *dda, int tile_px_size)
{
	dda->step = 1;
	while (dda->step <= tile_px_size)
	{
		dda->lx = center_px + (int)(cos(game->player.r) * dda->step);
		dda->ly = center_px + (int)(sin(game->player.r) * dda->step);
		dda->screen_x = MINIMAP_X + dda->lx;
		dda->screen_y = MINIMAP_Y + dda->ly;
		if (dda->screen_x >= MINIMAP_X && dda->screen_x < MINIMAP_X + 600
			&& dda->screen_y >= MINIMAP_Y && dda->screen_y < MINIMAP_Y + 600)
			mlx_put_pixel(game->frame, dda->screen_x,
				dda->screen_y, 0x00FF00FF);
		dda->step++;
	}
}

void	draw_player_square(t_game *game, int center_px, t_DDAmap *dda)
{
	dda->screen_x = MINIMAP_X + center_px + dda->dx;
	dda->screen_y = MINIMAP_Y + center_px + dda->dy;
	if (dda->screen_x >= MINIMAP_X && dda->screen_x < MINIMAP_X + 600
		&& dda->screen_y >= MINIMAP_Y && dda->screen_y < MINIMAP_Y + 600)
		mlx_put_pixel(game->frame, dda->screen_x, dda->screen_y, 0xFF0000FF);
	dda->dx++;
}

static void	draw_player_on_minimap(t_game *game, int center_px,
		int tile_px_size)
{
	t_DDAmap	dda;

	dda.psize = 3;
	dda.dy = -dda.psize / 2;
	while (dda.dy++ <= dda.psize / 2)
	{
		dda.dx = -dda.psize / 2;
		while (dda.dx++ <= dda.psize / 2)
			draw_player_square(game, center_px, &dda);
	}
	draw_player_vision_on_minimap(game, center_px, &dda, tile_px_size);
}

void	update_minimap(t_game *game)
{
	int	player_tile_x;
	int	player_tile_y;
	int	center_px;
	int	tile_px_size;

	player_tile_x = (int)game->player.pos.x;
	player_tile_y = (int)game->player.pos.y;
	tile_px_size = MINIMAP_SIZE_FIXED / (VIEW_RADIUS * 2 + 1);
	center_px = VIEW_RADIUS * tile_px_size;
	clear_minimap_background(game);
	draw_local_map_tiles(game, player_tile_x, player_tile_y, tile_px_size);
	draw_player_on_minimap(game, center_px, tile_px_size);
}
