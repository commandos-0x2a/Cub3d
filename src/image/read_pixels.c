/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_pixels.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 12:32:05 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/15 12:34:39 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sprite.h"
#include <stdlib.h>

static uint32_t	get_rgba(uint8_t index, t_rgb *palette, int trans)
{
	if (trans && index == 255)
		return (0);
	return ((((uint32_t *)palette)[index] << 8) | 255);
}

static void	convert_pixels(t_dspriteframe *header, uint32_t *pixels,
	uint8_t *pixels_data, t_rgb *palette)
{
	int			x;
	int			y;

	y = 0;
	while (y < header->height)
	{
		x = 0;
		while (x < header->width)
		{
			pixels[y * header->width + x] = get_rgba(
					pixels_data[y * header->width + x], palette, 1);
			x++;
		}
		y++;
	}
}

int	read_frame_pixels(int fd, uint32_t *pixels,
						t_rgb *palette,
						t_dspriteframe *header)
{
	uint8_t		*pixels_data;
	int			res;

	pixels_data = malloc(header->width * header->height);
	if (!pixels_data)
		return (SPRITE_ERROR_MEMORY);
	res = read_exact(fd, pixels_data, header->width * header->height);
	if (res != SPRITE_SUCCESS)
	{
		free(pixels_data);
		return (res);
	}
	convert_pixels(header, pixels, pixels_data, palette);
	free(pixels_data);
	return (SPRITE_SUCCESS);
}
