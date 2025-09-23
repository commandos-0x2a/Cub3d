/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_reader.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:00:00 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/23 19:33:55 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sprite.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

static int	read_exact(int fd, void *buffer, size_t size)
{
	ssize_t	bytes_read;
	ssize_t	total_read;
	char	*buf_ptr;

	if (!buffer)
		return (SPRITE_ERROR_NULL_PTR);
	buf_ptr = (char *)buffer;
	total_read = 0;
	while (total_read < (ssize_t)size)
	{
		bytes_read = read(fd, buf_ptr + total_read, size - total_read);
		if (bytes_read <= 0)
			return (SPRITE_ERROR_HEADER);
		total_read += bytes_read;
	}
	return (SPRITE_SUCCESS);
}

static int	validate_sprite_header(const t_sprite_header *sprite)
{
	const int	expected_id = 0x50534449;
	const int	expected_version = 2;

	if (!sprite)
		return (SPRITE_ERROR_NULL_PTR);
	if (sprite->id != expected_id)
		return (SPRITE_ERROR_INVALID);
	if (sprite->version != expected_version)
		return (SPRITE_ERROR_INVALID);
	if (sprite->palette_color_count != 256)
		return (SPRITE_ERROR_INVALID);
	if (sprite->nb_frame <= 0 || sprite->nb_frame > 1000)
		return (SPRITE_ERROR_INVALID);
	return (SPRITE_SUCCESS);
}

uint32_t	get_rgba(uint8_t index, uint8_t *palette, int trans)
{
	uint32_t	rgba;
	
	if (trans && index == 255)
		return (0);
	rgba = 0;
	rgba |= palette[index * 3 + 0] << 24;
	rgba |= palette[index * 3 + 1] << 16;
	rgba |= palette[index * 3 + 2] << 8;
	rgba |= 255;
	return (rgba);
}

static int	read_frame_pixels(int fd, uint32_t *pixels, 
						unsigned char *palette, 
						t_frame_header *header)
{
	uint8_t		*pixels_data;
	int			res;
	uint32_t	x;
	uint32_t	y;

	pixels_data = malloc(header->width * header->height);
	if (!pixels_data)
		return (SPRITE_ERROR_MEMORY);
	res = read_exact(fd, pixels_data, header->width * header->height);
	if (res != SPRITE_SUCCESS)
	{
		free(pixels_data);
		return (res);
	}
	y = 0;
	while (y < header->height)
	{
		x = 0;
		while (x < header->width)
		{
			pixels[y * header->width + x] = 
				get_rgba(pixels_data[y * header->width + x], palette, 1);
			x++;
		}
		y++;
	}
	free(pixels_data);
	return (SPRITE_SUCCESS);
}

static int	read_frames(int fd, t_sprite *spr)
{
	size_t			i;
	mlx_texture_t	*frame;
	int				res;
	t_frame_header	header;

	spr->frames = calloc(spr->header.nb_frame, sizeof(mlx_texture_t));
	if (!spr->frames)
		return (SPRITE_ERROR_MEMORY);
	i = 0;
	while (i < spr->header.nb_frame)
	{
		frame = &spr->frames[i];
		res = read_exact(fd, &header, sizeof(t_frame_header));
		if (res != SPRITE_SUCCESS)
			return (res);
		frame->width = header.width;
		frame->height = header.height;
		frame->bytes_per_pixel = sizeof(uint32_t);
		frame->pixels = malloc(header.width * header.height * sizeof(uint32_t));
		if (!frame->pixels)
			return (SPRITE_ERROR_MEMORY);
		res = read_frame_pixels(fd, (uint32_t *)frame->pixels, 
								spr->palette, &header);
		if (res != SPRITE_SUCCESS)
			return (res);
		
		i++;
	}
	return (SPRITE_SUCCESS);
}

int	load_sprite_file(int fd, t_sprite *spr)
{
	int	result;

	// read header 42 bytes
	result = read_exact(fd, &spr->header, sizeof(t_sprite_header));
	if (result != SPRITE_SUCCESS)
		return (result);

	// header validate
	result = validate_sprite_header(&spr->header);
	if (result != SPRITE_SUCCESS)
		return (result);
	
	// read palette 768 bytes
	result = read_exact(fd, spr->palette, sizeof(spr->palette));
	if (result != SPRITE_SUCCESS)
		return (result);

	result = read_frames(fd, spr);

	return (result);	
}
