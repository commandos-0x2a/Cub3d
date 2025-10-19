/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:43:23 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/19 16:04:56 by hassende         ###   ########.fr       */
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

typedef enum e_texture_index
{
	WALL_EAST,
	WALL_WEST,
	WALL_NORTH,
	WALL_SOUTH,
	WALL_DOOR,
	ICON_INDEX,
	TEXTURE_COUNT,
}	t_texture_index;

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
	int		step_x;
	int		step_y;
	int		side;
}	t_ray_cast;

typedef struct s_ray_hit
{
	float	distance;
	float	wall_x;
	int		tex_i;
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

	int					width;
	int					height;
	mlx_texture_t		*texture[TEXTURE_COUNT];
	int					is_animate;
	bool				interact;
}	t_game;

void			render(void *param);
void			player_control(void *param);
void			render_schema(t_game *game);

mlx_texture_t	*load_texture(t_game *game, const char *path, int i);
int				load_textures(t_game *game);
int				start_game(t_game *game, const char *map_path);
void			end_game(t_game *game, int status);
void			game_hooks(t_game *game);
int32_t			open_window(t_game *game);
t_ray_hit		cast_ray(t_game *game, double angle, int x);
int				get_tex_x(t_game *game, t_ray_hit *ray_hit);

#endif
