/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_test.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:00:00 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/19 19:52:09 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPRITE_H
# define SPRITE_H

# include <stdio.h>
# include <stdint.h>

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

typedef struct __attribute__((packed))	s_sprite_frame_header
{
	int	group;
	int	originX;
	int	originY;
	int	width;
	int	height;
}	t_sprite_frame_header;

int				validate_sprite_header(const t_sprite_header *sprite);

int				load_sprite_file(const char *filename, t_sprite_header *sprite,
					unsigned char *palette,
					t_sprite_frame_header *frame_header,
					unsigned char **pixel_data);

int				get_frames_by_group(const char *filename, int group_id,
					int **frame_indices, int *frame_count);

void			print_sprite_info(const t_sprite_header *sprite);
void			print_frame_info(const t_sprite_frame_header *frame_header);

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