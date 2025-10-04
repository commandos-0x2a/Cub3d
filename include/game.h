/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:43:23 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/01 21:49:56 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# include "map.h"
# include "utils.h"
# include <MLX42/MLX42.h>
# include <math.h>
# include <math.h>
# include "animate.h"

# define SAFE_MARGIN 0.35f  // Tune between 0.1 and 0.3
# define MINIMAP_SIZE 50
# define VIEW_RADIUS 3
# define TILE_COUNT 7
# define TILE_PX_SIZE 7
# define MINIMAP_X 10
# define MINIMAP_Y 10
# define INTERACT_RANGE 1.5f
# define PI 3.14159265359
# define FOV_RAD 1.0466666666666666666666666666667

enum e_direction
{
	FRONT,
	BACK,
	LEFT,
	RIGHT
};

typedef enum e_wall_side
{
	WALL_EAST,
	WALL_WEST,
	WALL_NORTH,
	WALL_SOUTH,
	WALL_DOOR,
}	t_wall_side;

typedef struct s_DDAmap
{
	int	dx;
	int	dy;
	int	psize;
	int	screen_x;
	int	screen_y;
	int	step;
	int	lx;
	int	ly;
}	t_DDAmap;

typedef struct s_tileMap
{
	int			dx;
	int			dy;
	int			map_x;
	int			map_y;
	int			tile_start_x;
	int			tile_start_y;
	int			screen_x;
	int			screen_y;
	int			px;
	int			py;
	int			map_w;
	int			map_h;
	char		tile;
	uint32_t	color;
}	t_tileMap;

typedef struct s_ray_cast
{
	float	px;
	float	py;
	float	dir_x;
	float	dir_y;
	int		map_x;
	int		map_y;
	float	delta_dist_x;
	float	delta_dist_y;
	float	side_dist_x;
	float	side_dist_y;
	float	step_x;
	float	step_y;
	int		side;
}	t_ray_cast;

typedef struct s_wall_draw
{
	float	corrected_distance;
	int		wall_height;
	int		wall_start;
	int		wall_end;
	int		x_start;
	int		x_end;
}	t_wall_draw;

typedef struct s_ray_hit
{
	float	distance;
	float	wall_x;			// texture coordinate along wall
	int		is_vertical;	// 1 if vertical wall, 0 if horizontal
}	t_ray_hit;

typedef struct s_debug
{
	double	sec;
	int		fps;
}	t_debug;

typedef struct s_player
{
	t_vector	pos;
	float		r;
	float		speed;
}	t_player;

typedef struct s_game
{
	t_map				*map;
	mlx_t				*mlx;
	mlx_image_t			*frame;
	t_player			player;
	t_debug				debug;

	size_t				nb_animate;
	t_sprite_animate	*animates[16];

	double				last_render;
	double				time_delta;

	int					rays_number;

	int					width;
	int					height;
	mlx_texture_t		*texture[6];
	bool				interact;
}	t_game;

int				max(int a, int b);
void			render(void *param);
void			player_control(void *param);
void			draw_player_vision(t_game *game);
uint32_t		get_pixel_color(mlx_texture_t *texture, int tex_x, int tex_y);
void			update_minimap(t_game *game);
int				load_textures(t_game *game);
t_game			*start_game(const char *map_path);
void			*end_game(t_game *game, int status);
void			game_hooks(t_game *game);
int32_t			open_window(t_game *game);
t_ray_hit		cast_ray(t_game *game, double angle);
mlx_texture_t	*get_wall_texture(t_game *game, t_ray_hit ray_hit);
void			init_wall_params(t_game *game, t_ray_hit ray_hit, int ray,
					t_wall_draw *wd);
void			calculate_distance_and_texture(t_ray_cast *rc, t_ray_hit *ray);
void			perform_dda(t_game *game, t_ray_cast *rc, t_ray_hit *ray);
void			set_step_and_side_dist(t_ray_cast *rc);
void			init_ray_params(t_game *game, double angle, t_ray_cast *rc);

#endif
