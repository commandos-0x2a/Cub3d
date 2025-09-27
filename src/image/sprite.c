/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:49:01 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/27 15:17:35 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sprite.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "animate.h"

void	free_group_frames(t_group_frame *gframes)
{
	int	i;

	i = 0;
	while (i < gframes->nb_frame)
	{
		free(gframes[i].frames[i].pixels);
		gframes[i].frames[i].pixels = NULL;
		i++;
	}
	free(gframes->frames);
	free(gframes->intervals);
}

void	free_sprite(t_sprite *spr)
{
	size_t	i;

	if (spr->frames)
	{
		i = 0;
		while (i < spr->nb_frame)
		{
			if (spr->frames[i].type == 2)
				free_group_frames(&spr->frames[i].u.group);
			else
			{
				free(spr->frames[i].u.single.pixels);
				spr->frames[i].u.single.pixels = NULL;
			}
		}
		free(spr->frames);
	}
	free(spr);
}

/**
 * @brief Get error message from error code
 * @param error_code Error code returned by sprite functions
 * @return String description of the error
 */
const char	*get_sprite_error_message(int error_code)
{
	if (error_code == SPRITE_SUCCESS)
		return ("Success");
	else if (error_code == SPRITE_ERROR_NULL_PTR)
		return ("Null pointer error");
	else if (error_code == SPRITE_ERROR_FILE_OPEN)
		return ("File open error");
	else if (error_code == SPRITE_ERROR_HEADER)
		return ("Header read error");
	else if (error_code == SPRITE_ERROR_INVALID)
		return ("Invalid sprite format");
	else if (error_code == SPRITE_ERROR_PALETTE)
		return ("Palette read error");
	else if (error_code == SPRITE_ERROR_FRAME)
		return ("Frame header read error");
	else if (error_code == SPRITE_ERROR_DIMENSIONS)
		return ("Invalid frame dimensions");
	else if (error_code == SPRITE_ERROR_MEMORY)
		return ("Memory allocation error");
	else if (error_code == SPRITE_ERROR_PIXEL_DATA)
		return ("Pixel data read error");
	else
		return ("Unknown error");
}

// void	sprite_next_frame(t_sprite *spr, uint32_t x, uint32_t y)
// {
// 	if ()
// 	spr->animate.tex.pixels = spr->frames[0];
// }



t_sprite	*load_sprite(const char *filename)
{
	t_sprite	*spr;
	int			fd;
	int			res;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (NULL);
	spr = malloc(sizeof(t_sprite));
	if (!spr)
	{
		close(fd);
		return (NULL);
	}
	spr->frames = NULL;
	res = load_sprite_file(fd, spr);
	close(fd);
	if (res != SPRITE_SUCCESS)
	{
		// free_sprite(spr);
		printf("Error read sprite (%s): %s\n", 
			filename, get_sprite_error_message(res));
		return (NULL);
	}
	return (spr);
}

