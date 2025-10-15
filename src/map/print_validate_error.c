/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_validate_error.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 11:59:53 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/15 12:51:56 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"
#include "libft.h"
#include <stdio.h>

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
			if (ft_strchr("01 D NSEW", c) == NULL)
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
