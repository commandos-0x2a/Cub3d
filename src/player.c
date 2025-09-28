#include "game.h"

#define PI 3.14159265359
#define MAX(a, b) (a > b ? a : b)
#define FOV_RAD (FOV * PI / 180.0)

static void	init_ray_params(t_game *game, double angle, t_ray_cast *rc)
{
	rc->px = game->player.pos.x;
	rc->py = game->player.pos.y;
	rc->dir_x = cos(angle);
	rc->dir_y = sin(angle);
	rc->map_x = (int)rc->px;
	rc->map_y = (int)rc->py;
	if (rc->dir_x == 0)
		rc->delta_dist_x = 1e30;
	else
		rc->delta_dist_x = fabs(1.0 / rc->dir_x);
	if (rc->dir_y == 0)
		rc->delta_dist_y = 1e30;
	else
		rc->delta_dist_y = fabs(1.0 / rc->dir_y);
}

static void	set_step_and_side_dist(t_ray_cast *rc)
{
	if (rc->dir_x < 0)
	{
		rc->step_x = -1;
		rc->side_dist_x = (rc->px - rc->map_x) * rc->delta_dist_x;
	}
	else
	{
		rc->step_x = 1;
		rc->side_dist_x = (rc->map_x + 1.0 - rc->px) * rc->delta_dist_x;
	}
	if (rc->dir_y < 0)
	{
		rc->step_y = -1;
		rc->side_dist_y = (rc->py - rc->map_y) * rc->delta_dist_y;
	}
	else
	{
		rc->step_y = 1;
		rc->side_dist_y = (rc->map_y + 1.0 - rc->py) * rc->delta_dist_y;
	}
}

static int	check_wall_hit(t_game *game, t_ray_cast *rc, t_ray_hit *ray)
{
	if (rc->map_x < 0 || rc->map_x >= (int)game->map->grid.w
		|| rc->map_y < 0 || rc->map_y >= (int)game->map->grid.h)
		return (1);
	if (game->map->grid.raw[rc->map_y * game->map->grid.w + rc->map_x] == '1')
		return (1);
	if (game->map->grid.raw[rc->map_y * game->map->grid.w + rc->map_x] == 'D')
	{
		ray->is_vertical = 5;
		return (1);
	}
	return (0);
}

static void	perform_dda(t_game *game, t_ray_cast *rc, t_ray_hit *ray)
{
	int	hit;

	hit = 0;
	while (!hit)
	{
		if (rc->side_dist_x < rc->side_dist_y)
		{
			rc->side_dist_x += rc->delta_dist_x;
			rc->map_x += (int)rc->step_x;
			rc->side = 0;
		}
		else
		{
			rc->side_dist_y += rc->delta_dist_y;
			rc->map_y += (int)rc->step_y;
			rc->side = 1;
		}
		hit = check_wall_hit(game, rc, ray);
	}
}

static void	calculate_distance_and_texture(t_ray_cast *rc, t_ray_hit *ray)
{
	if (rc->side == 0)
		ray->distance = (rc->map_x - rc->px + (1 - rc->step_x) / 2) / rc->dir_x;
	else
		ray->distance = (rc->map_y - rc->py + (1 - rc->step_y) / 2) / rc->dir_y;
	if (rc->side == 0)
		ray->wall_x = rc->py + ray->distance * rc->dir_y;
	else
		ray->wall_x = rc->px + ray->distance * rc->dir_x;
	ray->wall_x -= floor(ray->wall_x);
	if (ray->wall_x < 0)
		ray->wall_x += 1.0f;
}

static void	determine_wall_type(t_ray_cast *rc, t_ray_hit *ray)
{
	if (ray->is_vertical == 5)
		return ;
	if (rc->side == 0)
	{
		if (rc->step_x == 1)
			ray->is_vertical = WALL_EAST;
		else
			ray->is_vertical = WALL_WEST;
	}
	else
	{
		if (rc->step_y == 1)
			ray->is_vertical = WALL_SOUTH;
		else
			ray->is_vertical = WALL_NORTH;
	}
}

t_ray_hit	cast_ray(t_game *game, double angle)
{
	t_ray_hit	ray;
	t_ray_cast	rc;

	ray.is_vertical = 0;
	init_ray_params(game, angle, &rc);
	set_step_and_side_dist(&rc);
	perform_dda(game, &rc, &ray);
	calculate_distance_and_texture(&rc, &ray);
	determine_wall_type(&rc, &ray);
	return (ray);
}

uint32_t	get_pixel_color(mlx_texture_t *texture, int tex_x, int tex_y)
{
	uint8_t		*pixel;
	uint32_t	color;

	pixel = texture->pixels + (tex_y * texture->width + tex_x) * 4;
	color = (pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | pixel[3];
	return (color);
}

static void	init_wall_params(t_game *game, t_ray_hit ray_hit, int ray,
			t_wall_draw *wd)
{
	double	angle_offset;

	angle_offset = (ray - game->rays_number / 2) * (FOV * PI / 180.0
			/ game->rays_number);
	wd->corrected_distance = ray_hit.distance * cos(angle_offset);
	if (wd->corrected_distance <= 0.1f)
		wd->corrected_distance = 0.1f;
	wd->wall_height = (int)(game->height * TILE_SIZE / wd->corrected_distance);
	if (wd->wall_height > game->height)
		wd->wall_height = game->height;
	if (wd->wall_height < 1)
		wd->wall_height = 1;
	wd->wall_start = (game->height - wd->wall_height) / 2;
	wd->wall_end = wd->wall_start + wd->wall_height;
	wd->x_start = (ray * game->width) / game->rays_number;
	wd->x_end = ((ray + 1) * game->width) / game->rays_number;
}

static mlx_texture_t	*get_wall_texture(t_game *game, t_ray_hit ray_hit)
{
	mlx_texture_t	*texture;

	if (ray_hit.is_vertical == 5)
		texture = game->texture[4];
	else
		texture = game->texture[ray_hit.is_vertical];
	return (texture);
}

static void	draw_wall_column(t_game *game, t_wall_draw *wd,
			mlx_texture_t *texture, t_ray_hit ray_hit)
{
	int	x;
	int	y;
	int	tex_x;
	int	tex_y;

	x = wd->x_start;
	while (x < wd->x_end)
	{
		if (x >= 0 && x < game->width)
		{
			y = wd->wall_start;
			while (y < wd->wall_end)
			{
				if (y >= 0 && y < game->height)
				{
					tex_x = (int)(ray_hit.wall_x * texture->width);
					if (tex_x < 0)
						tex_x = 0;
					if (tex_x >= (int)texture->width)
						tex_x = texture->width - 1;
					tex_y = (int)((float)(y - wd->wall_start)
							/ (float)wd->wall_height * texture->height);
					if (tex_y < 0)
						tex_y = 0;
					if (tex_y >= (int)texture->height)
						tex_y = texture->height - 1;
					mlx_put_pixel(game->frame, x, y,
						get_pixel_color(texture, tex_x, tex_y));
				}
				y++;
			}
		}
		x++;
	}
}

void	draw_wall(t_game *game, t_ray_hit ray_hit, int ray)
{
	t_wall_draw		wd;
	mlx_texture_t	*texture;

	init_wall_params(game, ray_hit, ray, &wd);
	texture = get_wall_texture(game, ray_hit);
	if (!texture || !texture->pixels)
		return ;
	draw_wall_column(game, &wd, texture, ray_hit);
}

void	draw_player_vision(t_game *game)
{
	int			i;
	double		sangle;
	double		angle_step;
	double		current_angle;
	t_ray_hit	ray_hit;

	sangle = game->player.r - (FOV_RAD / 2.0);
	angle_step = FOV_RAD / game->rays_number;
	i = 0;
	while (i < game->rays_number)
	{
		current_angle = sangle + i * angle_step;
		ray_hit = cast_ray(game, current_angle);
		draw_wall(game, ray_hit, i);
		i++;
	}
}

void	update_player_pos(t_game *game)
{
	draw_player_vision(game);
	update_minimap(game);
}
