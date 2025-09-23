#include "game.h"
#include <stdio.h>

void	render_floor_ceiling(mlx_image_t *frame,
	uint32_t floor_color, uint32_t ceiling_color)
{
	size_t	y;
	size_t	x;

	y = 0;
	while (y < frame->height)
	{
		x = 0;
		while (x < frame->width)
		{
			if (y < frame->height / 2)
				mlx_put_pixel(frame, x, y, ceiling_color);
			else
				mlx_put_pixel(frame, x, y, floor_color);
			x++;
		}
		y++;
	}
}

void render(void* param)
{
	t_game	*game;
	double	now;

	game = param;
	// get delta time
	now = mlx_get_time();
	game->time_delta = now - game->last_render;
	game->last_render = now;
	game->debug.sec += game->time_delta;
	game->debug.fps++;
	if (game->debug.sec >= 1)
	{
		game->debug.sec = 0;
		game->debug.fps = 0;
	}
	mlx_delete_image(game->mlx, game->frame);
	game->frame = mlx_new_image(game->mlx, game->width, game->height);
	mlx_image_to_window(game->mlx, game->frame, 0, 0);
	render_floor_ceiling(game->frame, game->map->floor_color, game->map->ceiling_color);
	update_player_pos(game);
}
