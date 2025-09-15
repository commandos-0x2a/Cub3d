/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:43:23 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/15 14:17:22 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# include "map.h"
# include "utils.h"
# include <MLX42/MLX42.h>


enum	e_direction
{
	FRONT,
	BACK,
	LEFT,
	RIGHT
};

typedef enum	e_wall_side
{
	WALL_EAST,
	WALL_WEST,
	WALL_NORTH,
	WALL_SOUTH
}	t_wall_side;

typedef struct	s_ray_hit
{
	float	distance;
	float	wall_x;        // texture coordinate along wall
	int		is_vertical;   // 1 if vertical wall, 0 if horizontal
}	t_ray_hit;

typedef struct s_debug
{
	double	sec;
	int		fps;
}	t_debug;

typedef struct	s_player
{
	t_vector	pos;
	float		r;
	float		speed;
}	t_player;

typedef struct	s_game
{
	t_map			*map;
	mlx_t			*mlx;
	mlx_image_t		*frame;
	t_player		player;
	t_debug			debug;
	mlx_texture_t	*texture[4];
	double			last_render;
	double			time_delta;
	int				rays_number;
	int				width;
	int				height;
}	t_game;

void			render(void* param);
void			player_control(void *param);
unsigned long	time_now_ms(void);
void			update_player_pos(t_game *game);
uint32_t    	get_pixel_color(mlx_texture_t *texture, int tex_x, int tex_y);

#endif
