#include "game.h"
#include <stdio.h>
#include <math.h>

# define PI 3.14

int abs(int n) { return n > 0 ? n : n * -1; }

int hit_wall(float x, float y)
{
	int	mx;
	int	my;

	// ? 120 stands for the tile size, reducing this will result in out of bounds
	mx = (int)(x / TILE_SIZE);
	my = (int)(y / TILE_SIZE);
    // Check bounds
	// ! please replace these values. these are for debugging
    if (mx < 0 || mx >= 9 || my < 0 || my >= 6)
		return 1; // Out of bounds = wall

	return (map[my][mx] == 1);
}

float cast_ray(t_game *game, double angle, int draw_ray)
{
	int const   max_distance = 1000;
	float       px;
	float       py;
	float       rcos;
	float       rsin;
	float       step_size;
	float       distance;
	float		rx;
	float		ry;

	px = WIDTH / 2 + game->player.pos.x * 10 + PIXEL_SCALE;
	py = HEIGHT / 2 + game->player.pos.y * 10 + PIXEL_SCALE;
	rcos = cos(angle);
	rsin = sin(angle);
	step_size = 1;
	distance = 0;
	while (distance < max_distance)
	{
		rx = px + rcos * distance;
		ry = py + rsin * distance;
		if (hit_wall(rx, ry))
		{
			if (draw_ray && rx > 0 && ry > 0 && rx < WIDTH && ry < HEIGHT)
				mlx_put_pixel(game->frame, (int)rx, (int)ry, 0x00FF00FF);
			return (distance);
		}
		if (draw_ray && rx > 0 && ry > 0 && rx < WIDTH && ry < HEIGHT)
				mlx_put_pixel(game->frame, (int)rx, (int)ry, 0x00FF00FF);
		distance += step_size;
	}

	return (max_distance); // incase no wall is hit
}

void	draw_wall(t_game *game, float wall_distance, int ray)
{
	// ! In debug mode
	int const color = 0xFF00FFFF;
	// int const max_distance = 1000;
	// (void)ray;
	int new_width = WIDTH / 2;
	int	i;
	int	y;
	int	wall_height;
	int	wall_start;
	int	wall_end;
	int	x_segment;

	i = 0;
	y = 0;
	x_segment = (new_width / NUM_RAYS);
	wall_height = (int)(new_width / wall_distance * 50);
	double angle_diff = (ray - NUM_RAYS/2) * (FOV * PI / 180) / NUM_RAYS;
    wall_height *= cos(angle_diff);
	wall_start = ((HEIGHT - wall_height) / 2);
	wall_end = wall_start + wall_height;
	while (i < x_segment)
	{
		y = 0;
		while (y < HEIGHT)
		{
			int x = ray * x_segment + i;
			if (y >= wall_start && y <= wall_end)
				mlx_put_pixel(game->frame, x + new_width, y, color);
			y++;
		}
		i++;
	}
}

// ! remove before eval !!!!
// This function uses DDA to detemine the player's vision
// Where FOV is the range the player can see
void draw_player_vision(t_game *game)
{
	int			i;
	double		fov_rad;
	double		sangle;
	double		angle_step;
	double		current_angle;
	float		wall_distance;

	fov_rad = FOV * PI / 180;
	sangle = game->player.r - (fov_rad / 2);
	angle_step = fov_rad / NUM_RAYS;
	i = 0;
	while (i < NUM_RAYS)
	{
		current_angle = sangle + (i * angle_step);
		wall_distance = cast_ray(game, current_angle, 1); // 1 means draw the ray. For debugging
		draw_wall(game, wall_distance, i);
		i++;
	}

}

void update_player_pos(t_game *game)
{
	int base_px = WIDTH / 2 + game->player.pos.x * PIXEL_SCALE;
	int base_py = HEIGHT / 2 + game->player.pos.y * PIXEL_SCALE;

	int const player_size = 20;
	int i = 0;
	int j = 0;
	int px = 0;
	int py = 0;
	while (i < player_size)
	{
		py = base_py + i;
		while (j < player_size)
		{
			px = base_px + j;
			if ((px < WIDTH && py < HEIGHT) && (px > 0 && py > 0))
				mlx_put_pixel(game->frame, px, py, 0x4287f5);
			j++;
		}
		j = 0;
		i++;
	}
	draw_player_vision(game);
}
