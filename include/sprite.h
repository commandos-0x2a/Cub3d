/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:00:00 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/23 18:39:57 by yaltayeh         ###   ########.fr       */
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


typedef	struct __attribute__((packed))	s_frame_header
{
	int	group;
	int	originX;
	int	originY;
	int	width;
	int	height;
} t_frame_header;

typedef struct s_sprite
{
	t_animate		animate;
	t_sprite_header	header;
	unsigned char	palette[256 * 3];
	mlx_texture_t	*frames;
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