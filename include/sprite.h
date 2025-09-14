/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 21:39:06 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/14 21:49:41 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPRITE_H
# define SPRITE_H

typedef struct	s_sprite
{
	int		id;						// format ID, "IDSP" (0x49 0x44 0x53 0x50)
	int		version;				// Format version number. HL1 SPRs are version 2 (0x02,0x00,0x00,0x00)
	int		type;					// Orientation method
	int		text_format;			// Translucency/Transparency method
	float	bounding_radius;
	int		max_width;
	int		max_height;
	int		nb_frame;				// number of frames the sprite contains
	float	beam_length;
	int		synch_type;
	short	palette_color_count;	// number of colors in the palette; should be 256
}	t_sprite;

typedef struct	s_sprite_frame_header
{
	int	group;
	int	originX;		// not sure about this one, it always huge for sprites in HL1
	int	originY;
	int	width;
	int	height;

	// Right after this, the paletted image data comes, each byte is a pixel.
	// The image size is given in the frame header, so the entire data for 1 frame is width*height bytes.
}	t_sprite_frame_header;

#endif
