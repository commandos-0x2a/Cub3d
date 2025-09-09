/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:43:23 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/09 16:08:59 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# include "map.h"
# include "utils.h"
# include <MLX42/MLX42.h>

# define WIDTH 1080
# define HEIGHT 720

enum	e_direction
{
	FRONT,
	BACK,
	LEFT,
	RIGHT
};

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
	t_player		*player;
	unsigned long	last_render;
	unsigned long	time_delta;
}	t_game;

void			render(void* param);
void			player_control(void *param);
unsigned long	time_now_ms(void);

#endif
