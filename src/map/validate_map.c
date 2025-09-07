/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 16:04:17 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/06 11:34:01 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"
#include "libft.h"
#include <stdio.h>

void	print_valid_char_error(t_map *map)
{
	size_t	y;
	size_t	x;
	char	c;
	
	y = 0;
	while (y < map->h)
	{
		x = 0;
		while (x < map->w)
		{
			c = map->grid[y * map->w + x];
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
}

int	valid_char(t_map *map)
{
	size_t	y;
	size_t	x;
	char	c;
	
	y = 0;
	while (y < map->h)
	{
		x = 0;
		while (x < map->w)
		{
			c = map->grid[y * map->w + x];
			if (ft_strchr("01 NSEW", c) == NULL)
			{
				print_valid_char_error(map);
				return (0);
			}
			x++;
		}
		y++;
	}
	return (1);
}

int	valid_player_number(t_map *map)
{
  (void)map;
	return (1);
}

int	validate_map(t_map *map)
{
	if (valid_char(map) == 0)
		return (0);
	return (1);
}
