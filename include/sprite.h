/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:00:00 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/24 12:05:57 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPRITE_TEST_H
# define SPRITE_TEST_H

# include <stdio.h>
# include <stdint.h>
# include <MLX42/MLX42.h>
# include "animate.h"

typedef struct __attribute__((packed))	s_sprite_header
{
	int		id;
	int		version;
	int		type;
	int		text_format;
	float	bounding_radius;
	int		max_width;
	int		max_height;
	int		nb_frame;
	float	beam_length;
	int		synch_type;
	short	palette_color_count;
}	t_sprite_header;

typedef struct __attribute__((packed))	s_dspriteframe
{
	int		origin[2];  // X and Y offset from the origin (usually [0,0])
	int		width;      // Width of the frame in pixels
	int		height;     // Height of the frame in pixels
}	t_dspriteframe;

typedef struct s_group_frame
{
	int				nb_frame;
	float			*intervals;
	mlx_texture_t	*frames;
}	t_group_frame;

typedef struct s_rgb
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
}	t_rgb;

typedef struct s_frame
{
	int	type;
	union u_frame
	{
		t_group_frame	group;
		mlx_texture_t	single;
	}	u;
}	t_frame;

typedef struct s_sprite
{
	t_animate		animate;
	int				max_width;
	int				max_height;
	int				text_format;
	
	t_rgb			palette[256];

	size_t			nb_frame;
	t_frame			*frames;
}	t_sprite;

int			load_sprite_file(int fd, t_sprite *spr);
void		free_sprite(t_sprite *spr);
t_sprite	*load_sprite(const char *filename);

# define SPRITE_SUCCESS			0
# define SPRITE_ERROR_NULL_PTR		-1
# define SPRITE_ERROR_FILE_OPEN		-2
# define SPRITE_ERROR_HEADER		-3
# define SPRITE_ERROR_INVALID		-4
# define SPRITE_ERROR_PALETTE		-5
# define SPRITE_ERROR_FRAME		-6
# define SPRITE_ERROR_DIMENSIONS	-7
# define SPRITE_ERROR_MEMORY		-8
# define SPRITE_ERROR_PIXEL_DATA	-9

#endif