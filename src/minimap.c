#include "game.h"

#define MINIMAP_SIZE game->width / 6
#define VIEW_RADIUS 3
#define TILE_COUNT (VIEW_RADIUS * 2 + 1)
#define TILE_PX_SIZE (MINIMAP_SIZE / TILE_COUNT)
#define MINIMAP_X 10
#define MINIMAP_Y 10

static void	clear_minimap_background(t_game *game, int minimap_x, int minimap_y,
		int minimap_size)
{
	int	i;
	int	j;

	i = 0;
	while (i < minimap_size)
	{
		j = 0;
		while (j < minimap_size)
		{
			mlx_put_pixel(game->frame, minimap_x + i, minimap_y + j, 0x000000FF);
			j++;
		}
		i++;
	}
}

static void	draw_local_map_tiles(t_game *game, int minimap_x, int minimap_y,
		int tile_px_size, int player_tile_x, int player_tile_y, int view_radius)
{
	int	dx;
	int	dy;
	int	map_x;
	int	map_y;
	int	tile_start_x;
	int	tile_start_y;
	int	screen_x;
	int	screen_y;
	int	px;
	int	py;
	int	map_w;
	int	map_h;
	char	tile;
	uint32_t color;

	map_w = game->map->grid.w;
	map_h = game->map->grid.h;
	dy = -view_radius;
	while (dy <= view_radius)
	{
		dx = -view_radius;
		while (dx <= view_radius)
		{
			map_x = player_tile_x + dx;
			map_y = player_tile_y + dy;
			if (map_x >= 0 && map_x < map_w && map_y >= 0 && map_y < map_h)
			{
				tile = game->map->grid.raw[map_y * map_w + map_x];
				color = (tile == '1') ? 0xFFFFFFFF : 0x444444FF;
				tile_start_x = (dx + view_radius) * tile_px_size;
				tile_start_y = (dy + view_radius) * tile_px_size;
				py = 0;
				while (py < tile_px_size)
				{
					px = 0;
					while (px < tile_px_size)
					{
						screen_x = minimap_x + tile_start_x + px;
						screen_y = minimap_y + tile_start_y + py;
						if (screen_x >= minimap_x
							&& screen_x < minimap_x + (view_radius * 2 + 1)
								* tile_px_size
							&& screen_y >= minimap_y
							&& screen_y < minimap_y + (view_radius * 2 + 1)
								* tile_px_size)
							mlx_put_pixel(game->frame, screen_x, screen_y, color);
						px++;
					}
					py++;
				}
			}
			dx++;
		}
		dy++;
	}
}

static void	draw_player_on_minimap(t_game *game, int minimap_x, int minimap_y,
		int center_px, int tile_px_size)
{
	int	dx;
	int	dy;
	int	psize;
	int	screen_x;
	int	screen_y;
	int	step;
	int	lx;
	int	ly;

	psize = 3;
	dy = -psize / 2;
	while (dy <= psize / 2)
	{
		dx = -psize / 2;
		while (dx <= psize / 2)
		{
			screen_x = minimap_x + center_px + dx;
			screen_y = minimap_y + center_px + dy;
			if (screen_x >= minimap_x && screen_x < minimap_x + 600
				&& screen_y >= minimap_y && screen_y < minimap_y + 600)
				mlx_put_pixel(game->frame, screen_x, screen_y, 0xFF0000FF);
			dx++;
		}
		dy++;
	}

	step = 1;
	while (step <= tile_px_size)
	{
		lx = center_px + (int)(cos(game->player.r) * step);
		ly = center_px + (int)(sin(game->player.r) * step);
		screen_x = minimap_x + lx;
		screen_y = minimap_y + ly;
		if (screen_x >= minimap_x && screen_x < minimap_x + 600
			&& screen_y >= minimap_y && screen_y < minimap_y + 600)
			mlx_put_pixel(game->frame, screen_x, screen_y, 0x00FF00FF);
		step++;
	}
}

void	update_minimap(t_game *game)
{
	int	player_tile_x;
	int	player_tile_y;
	int	center_px;

	player_tile_x = (int)game->player.pos.x;
	player_tile_y = (int)game->player.pos.y;
	center_px = VIEW_RADIUS * TILE_PX_SIZE;

	clear_minimap_background(game, MINIMAP_X, MINIMAP_Y, MINIMAP_SIZE);
	draw_local_map_tiles(game, MINIMAP_X, MINIMAP_Y, TILE_PX_SIZE,
		player_tile_x, player_tile_y, VIEW_RADIUS);
	draw_player_on_minimap(game, MINIMAP_X, MINIMAP_Y, center_px, TILE_PX_SIZE);
}
