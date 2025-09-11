/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:04:17 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/09 21:27:11 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"
#include "libft.h"
#include <stdio.h>

int	valid_file_name(const char *map_file)
{
	char	*ext;

	ext = ft_strrchr(map_file, '.');
	if (!ext)
		return (0);
	return (ft_strcmp(ext, ".cub") == 0);
}

int	print_valid_char_error(t_grid *grid)
{
	size_t	y;
	size_t	x;
	char	c;
	
	y = 0;
	while (y < grid->h)
	{
		x = 0;
		while (x < grid->w)
		{
			c = grid->raw[y * grid->w + x];
			if (ft_strchr("01 NSEW", c) == NULL)
				printf(RED"%c"RESET, c);
			else
				printf("%c", c);
			x++;
		}
		printf("\n");
		y++;
	}
	printf(RED"Error: found char not valid in map\n\n"RESET);
	return (0);
}

int	print_nb_player_error(t_grid *grid, int nb_player)
{
	size_t	y;
	size_t	x;
	char	c;

	y = 0;
	while (y < grid->h)
	{
		x = 0;
		while (x < grid->w)
		{
			c = grid->raw[y * grid->w + x];
			if (ft_strchr("NSEW", c))
				printf(RED"%c"RESET, c);
			else
				printf("%c", c);
			x++;
		}
		printf("\n");
		y++;
	}
	if (nb_player == 0)
		printf(RED"Error: not found player in map\n\n"RESET);
	else
		printf(RED"Error: too many player in map\n\n"RESET);
	return (0);
}

int	valid_char(t_grid *grid)
{
	size_t	y;
	size_t	x;
	char	c;
	int		nb_player;

	y = 0;
	nb_player = 0;
	while (y < grid->h)
	{
		x = 0;
		while (x < grid->w)
		{
			c = grid->raw[y * grid->w + x];
			if (ft_strchr("01 ", c))
				;
			else if (ft_strchr("NSEW", c))
				nb_player++;
			else
				return (print_valid_char_error(grid));
			x++;
		}
		y++;
	}
	if (nb_player != 1)
	{
		print_nb_player_error(grid, nb_player);
		return (0);
	}
	return (1);
}

int	validate_map(t_map *map)
{
	if (valid_char(&map->grid) == 0)
		return (0);
	if (valid_surrounded_wall(map) == 0)
		return (0);
	return (1);
}
