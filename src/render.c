#include "game.h"

void render_world(t_game *game)
{
	(void)game;
}

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
	t_game* game;

	game = param;
	render_floor_ceiling(game->frame, game->map->floor_color, game->map->ceiling_color);
	render_world(game);
}
