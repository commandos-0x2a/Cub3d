/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_schema_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 08:47:44 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/25 08:06:17 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_schema.h"

int	open_xpm_file(t_image *image,\
						char *filename, const char *img_name)
{
	mlx_texture_t	*tex;

	ft_strlcpy((char *)image, img_name, NAME_SIZE);
	tex = (void *)mlx_load_xpm42(filename);
	if (!tex)
	{
		printf("error: %s\n", filename);
		return (-1);
	}
	image->tex = *tex;
	free(tex);
	return (0);
}

int	open_png_file(t_image *image,\
						char *filename, const char *img_name)
{
	mlx_texture_t	*tex;

	ft_strlcpy((char *)image, img_name, NAME_SIZE);
	tex = mlx_load_png(filename);
	if (!tex)
	{
		printf("error: %s\n", filename);
		return (-1);
	}
	image->tex = *tex;
	free(tex);
	return (0);
}

t_object	*pop_player(t_object *components)
{
	t_object	*current;
	t_object	*prev;

	current = components->childrens;
	prev = NULL;
	while (current)
	{
		if (is_object_type(current, "player"))
		{
			if (!prev && !current->next)
				return (NULL);
			if (!prev)
				components->childrens = current->next;
			else
				prev->next = current->next;
			current->next = NULL;
			return (current);
		}
		prev = current;
		current = current->next;
	}
	return (NULL);
}

void	insert_player(t_object *current, t_object *prev, \
						t_object *player, t_object *components)
{
	if (!prev)
		add_children(components, player);
	else
	{
		player->next = current;
		prev->next = player;
	}
}
