/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:04:17 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/15 12:00:24 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"
#include "libft.h"
#include <stdio.h>

int	print_valid_char_error(t_grid *grid);
int	print_nb_player_error(t_grid *grid, int nb_player);

int	valid_file_name(const char *map_file)
{
	char	*ext;

	ext = ft_strrchr(map_file, '.');
	if (!ext)
		return (0);
	return (ft_strcmp(ext, ".cub") == 0);
}

static int	valid_char(t_grid *grid)
{
	size_t	y;
	size_t	x;
	int		nb_player;

	y = 0;
	nb_player = 0;
	while (y < grid->h)
	{
		x = 0;
		while (x < grid->w)
		{
			if (ft_strchr("01 D", grid->raw[y * grid->w + x]))
				;
			else if (ft_strchr("NSEW", grid->raw[y * grid->w + x]))
				nb_player++;
			else
				return (print_valid_char_error(grid));
			x++;
		}
		y++;
	}
	if (nb_player != 1)
		return (print_nb_player_error(grid, nb_player));
	return (1);
}

static void	get_player_init(t_grid *grid,
	int *player_x,
	int *player_y,
	char *player_r)
{
	size_t	x;
	size_t	y;

	y = 0;
	while (y < grid->h)
	{
		x = 0;
		while (x < grid->w)
		{
			if ((ft_strchr("WESN", grid->raw[y * grid->w + x])))
			{
				*player_x = x;
				*player_y = y;
				*player_r = grid->raw[y * grid->w + x];
			}
			x++;
		}
		y++;
	}
}

int	valid_color(int ceiling_color, int floor_color)
{
	int	err;

	err = 1;
	if (ceiling_color == 0)
	{
		err = 0;
		printf("please enter ceiling_color use 'C' identifier\n");
	}
	if (floor_color == 0)
	{
		err = 0;
		printf("please enter floor_color use 'F' identifier\n");
	}
	return (err);
}

int	validate_map(t_map *map)
{
	if (valid_char(&map->grid) == 0)
		return (0);
	if (valid_surrounded_wall(map) == 0)
		return (0);
	if (valid_color(map->ceiling_color, map->floor_color) == 0)
		return (0);
	get_player_init(&map->grid,
		&map->player_x,
		&map->player_y,
		&map->player_r);
	return (1);
}
