/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:49:01 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/15 12:18:39 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sprite.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "animate.h"
#include <libft.h>

void	free_group_frames(t_group_frame *gframes)
{
	int	i;

	i = 0;
	while (i < gframes->nb_frame)
	{
		free(gframes->frames[i].pixels);
		gframes->frames[i].pixels = NULL;
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
			i++;
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
const char	*get_sprite_error_message(size_t error_code)
{
	static const char	*sprite_error_messages[] = {
		"Success",
		"Null pointer error",
		"File open error",
		"Header read error",
		"Invalid sprite format",
		"Palette read error",
		"Frame header read error",
		"Invalid frame dimensions",
		"Memory allocation error",
		"Pixel data read error"
	};

	if (error_code >= sizeof(sprite_error_messages)
		/ sizeof(sprite_error_messages[0]))
		return ("Unknown error");
	return (sprite_error_messages[error_code]);
}

t_sprite	*load_sprite(const char *filename)
{
	t_sprite	*spr;
	int			fd;
	int			res;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (NULL);
	spr = ft_calloc(1, sizeof(t_sprite));
	if (!spr)
	{
		close(fd);
		return (NULL);
	}
	res = load_sprite_file(fd, spr);
	close(fd);
	if (res != SPRITE_SUCCESS)
	{
		free_sprite(spr);
		printf("Error: load sprite (%s): %s\n",
			filename, get_sprite_error_message(res));
		return (NULL);
	}
	return (spr);
}
