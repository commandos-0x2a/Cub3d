/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_reader.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:00:00 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/15 12:34:06 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sprite.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <libft.h>

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

int	read_single_frame(int fd, mlx_texture_t *frame, t_rgb *palette)
{
	t_dspriteframe	header;
	int				res;

	res = read_exact(fd, &header, sizeof(header));
	if (res != SPRITE_SUCCESS)
		return (res);
	frame->width = header.width;
	frame->height = header.height;
	frame->bytes_per_pixel = sizeof(uint32_t);
	frame->pixels = malloc(header.width * header.height * sizeof(uint32_t));
	if (!frame->pixels)
		return (SPRITE_ERROR_MEMORY);
	res = read_frame_pixels(fd, (uint32_t *)frame->pixels,
			palette, &header);
	return (res);
}

int	read_group_frame(int fd, t_group_frame *group, t_rgb *palette)
{
	int				i;
	int				res;

	res = read_exact(fd, &group->nb_frame, sizeof(group->nb_frame));
	if (res != SPRITE_SUCCESS)
		return (res);
	group->intervals = ft_calloc(group->nb_frame, sizeof(*group->intervals));
	if (!group->intervals)
		return (SPRITE_ERROR_MEMORY);
	res = read_exact(fd, group->intervals,
			sizeof(*group->intervals) * group->nb_frame);
	if (res != SPRITE_SUCCESS)
		return (res);
	group->frames = ft_calloc(group->nb_frame, sizeof(*group->frames));
	if (!group->frames)
		return (SPRITE_ERROR_MEMORY);
	i = 0;
	while (i < group->nb_frame)
	{
		res = read_single_frame(fd, &group->frames[i], palette);
		if (res != SPRITE_SUCCESS)
			return (res);
		i++;
	}
	return (SPRITE_SUCCESS);
}

static int	read_frames(int fd, t_sprite *spr)
{
	size_t			i;
	t_frame			*frame;
	int				res;

	spr->frames = ft_calloc(spr->nb_frame, sizeof(*spr->frames));
	if (!spr->frames)
		return (SPRITE_ERROR_MEMORY);
	i = 0;
	while (i < spr->nb_frame)
	{
		frame = &spr->frames[i];
		res = read_exact(fd, &frame->type, sizeof(frame->type));
		if (res != SPRITE_SUCCESS)
			return (res);
		if (frame->type == 2)
			res = read_group_frame(fd, &frame->u.group, spr->palette);
		else
			res = read_single_frame(fd, &frame->u.single, spr->palette);
		if (res != SPRITE_SUCCESS)
			return (res);
		i++;
	}
	return (SPRITE_SUCCESS);
}

int	load_sprite_file(int fd, t_sprite *spr)
{
	int				result;
	t_sprite_header	header;

	result = read_exact(fd, &header, sizeof(t_sprite_header));
	if (result != SPRITE_SUCCESS)
		return (result);
	result = validate_sprite_header(&header);
	if (result != SPRITE_SUCCESS)
		return (result);
	spr->max_width = header.max_width;
	spr->max_height = header.max_height;
	spr->text_format = header.text_format;
	spr->nb_frame = header.nb_frame;
	result = read_exact(fd, spr->palette, sizeof(spr->palette));
	if (result != SPRITE_SUCCESS)
		return (result);
	result = read_frames(fd, spr);
	return (result);
}
