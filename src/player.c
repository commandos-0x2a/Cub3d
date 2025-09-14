#include "game.h"
#include <stdio.h>
#include <math.h>

#define PI 3.14159265359

int abs(int n) { return n > 0 ? n : n * -1; }

int hit_wall(t_game *game, float x, float y)
{
	int mx = (int)x;
	int my = (int)y;

	if (mx < 0 || mx >= game->map->grid.w || my < 0 || my >= game->map->grid.h)
		return 1;

	return (game->map->grid.raw[my * game->map->grid.w + mx] == '1');
}

t_ray_hit cast_ray(t_game *game, double angle, int draw_ray)
{
	int const max_distance = 1000;
	float px = game->player.pos.x; // world space
	float py = game->player.pos.y;
	float rcos = cos(angle);
	float rsin = sin(angle);
	float step_size = 0.1f; // smaller step for accuracy
	float distance = 0;
	t_ray_hit ray;

	while (distance < max_distance)
	{
		float rx = px + rcos * distance;
		float ry = py + rsin * distance;

		if (hit_wall(game, rx, ry))
		{
			ray.distance = distance;

			// Convert to texture coordinate
			float world_rx = rx;
			float world_ry = ry;

			// For vertical walls, use Y fractional part
			ray.wall_x = fmod(world_ry, 1.0f);

			return ray;
		}

		distance += step_size;
	}

	ray.distance = max_distance;
	return ray;
}

void draw_wall(t_game *game, t_ray_hit ray_hit, int ray)
{
	int color = 0xFF00FFFF;
	int x;
	int y;
	int wall_height;
	int wall_start;
	int wall_end;
	int x_start;
	int x_end;
	double angle_offset;
	float corrected_distance;

	// fish effect correction
	angle_offset = (ray - NUM_RAYS / 2) * (FOV * PI / 180.0 / NUM_RAYS);
	corrected_distance = ray_hit.distance * cos(angle_offset);
	wall_height = (int)((HEIGHT * TILE_SIZE / corrected_distance));
	if (wall_height > HEIGHT)
		wall_height = HEIGHT;
	if (wall_height < 1)
		wall_height = 1;
	wall_start = ((HEIGHT - wall_height) / 2);
	wall_end = wall_start + wall_height;
	x_start = (ray * WIDTH) / NUM_RAYS;
	x_end = ((ray + 1) * WIDTH) / NUM_RAYS;
	x = x_start;
	mlx_texture_t *texture = game->texture[0];
	while (x < x_end)
	{
		y = wall_start;
		while (y < wall_end)
		{
			if (x > 0 && x <= WIDTH && y > 0 && y <= HEIGHT)
			{
				int tex_x = (int)(ray_hit.wall_x * texture->width);
				if (tex_x < 0)
					tex_x = 0;
				if (tex_x >= (int)texture->width)
					tex_x = texture->width - 1;

				int tex_y = ((y - wall_start) * texture->height) / wall_height;
				if (tex_y < 0)
					tex_y = 0;
				if (tex_y >= (int)texture->height)
					tex_y = texture->height - 1;
				uint8_t *pixel = texture->pixels + (tex_y * texture->width + tex_x) * 4;
				uint32_t color = (pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | pixel[3];
				mlx_put_pixel(game->frame, x, y, color);
			}
			y++;
		}
		x++;
	}
}

// ! remove before eval !!!!
// This function uses DDA to detemine the player's vision
// Where FOV is the range the player can see
void draw_player_vision(t_game *game)
{
	int i;
	double fov_rad;
	double sangle;
	double angle_step;
	double current_angle;
	t_ray_hit ray_hit;

	fov_rad = FOV * PI / 180;
	sangle = game->player.r - (fov_rad / 2);
	angle_step = fov_rad / NUM_RAYS;
	i = 0;
	while (i < NUM_RAYS)
	{
		current_angle = sangle + (i * angle_step);
		ray_hit = cast_ray(game, current_angle, 1); // 1 means draw the ray. For debugging
		draw_wall(game, ray_hit, i);
		i++;
	}
}

void update_player_pos(t_game *game)
{
	draw_player_vision(game);
}
