/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:43:23 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/09 18:52:37 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# include "map.h"
# include "utils.h"
# include <MLX42/MLX42.h>

// Resolution
# define WIDTH 1080
# define HEIGHT 720

// PIXEL SCALE: is for scaling the entites movement. because moving 1px to right in a 1080 720 res is slow as hell
# define PIXEL_SCALE 10

// ( int ) FOV: determines the range the player will see from.
// higher number results in more angle, less number will result in less angle to see through
// Giving the "zoomed" effect
# define FOV 60

# define TILE_SIZE 60

// NUM_RAYS: the amount of rays that will be shot from the player. More gives more detailed and less prone to hidden walls or entites
// due to the lack of rays.
# define NUM_RAYS 300

//quick debug map
extern int map[6][9];

enum	e_direction
{
	FRONT,
	BACK,
	LEFT,
	RIGHT
};

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
	t_map		*map;
	mlx_t		*mlx;
	mlx_image_t	*frame;
	t_player	player;
	t_debug		debug;
	double		last_render;
	double		time_delta;
}	t_game;

void			render(void* param);
void			player_control(void *param);
unsigned long	time_now_ms(void);
void			update_player_pos(t_game *game);

#endif
