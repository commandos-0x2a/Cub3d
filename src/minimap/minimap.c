/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 20:20:30 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/07 14:19:28 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

#define MINIMAP_SIZE_FIXED 150

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
