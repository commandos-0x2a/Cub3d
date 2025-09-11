#include "game.h"
#include <stdio.h>

int map[6][9]=
{
	{1, 1, 1, 1, 1, 1, 1, 1, 1}
	,{1, 0, 0, 0, 0, 0, 0, 1, 1}
	,{1, 0, 1, 1, 1, 0, 0, 1, 1}
	,{1, 0, 0, 0, 0, 0, 0, 1, 1}
	,{1, 0, 0, 0, 1, 0, 0, 1, 1}
	,{1, 1, 1, 1, 1, 1, 1, 1, 1}
};

void render_world(t_game *game)
{
    int tile_size = 120;
    int black = 0x000000FF;

	int	i;
	int	j;

	i = 0;
	while (i < 6)
	{
		j = 0;
		while (j < 9)
		{
			if (map[i][j] == 1)
			{
				// draw a square
				int start_x = j * tile_size;
                int start_y = i * tile_size;

                for (int py = start_y; py < start_y + tile_size - 1; py++)
                {
                    for (int px = start_x; px < start_x + tile_size - 1; px++)
                    {
                        // Add bounds checking to prevent segfault
                        if (px >= 0 && px < (int)game->frame->width &&
                            py >= 0 && py < (int)game->frame->height)
                        {
                            mlx_put_pixel(game->frame, px, py, black);
                        }
                    }
                }
			}
			j++;
		}
		i++;
	}
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
	t_game	*game;
	double	now;

	game = param;

	// get delta time
	now = mlx_get_time();
	game->time_delta = now - game->last_render;
	game->last_render = now;

	// render_floor_ceiling(game->frame, game->map->floor_color, game->map->ceiling_color);
	// render_world(game);
	// // ? temporary player position update

	// ! Debug window for now
	mlx_delete_image(game->mlx, game->frame);
	game->frame = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	mlx_image_to_window(game->mlx, game->frame, 0, 0);
	update_player_pos(game);
	render_world(game);

	// ! This laggens the game with time
	// char buf[1024];
	// snprintf(buf, 1024, "player: (%.2f, %.2f)", game->player.pos.x, game->player.pos.y);
	// printf("player: (%.2f, %.2f)\n", game->player.pos.x, game->player.pos.y);
	// mlx_put_string(game->mlx, buf, 10, 10);
}
