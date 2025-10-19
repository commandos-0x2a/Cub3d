/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surrounded_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 12:01:57 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/19 15:21:46 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"
#include "libft.h"
#include <stdio.h>

void	get_player_position(t_grid *grid, int *px, int *py)
{
	int	y;
	int	x;

	y = 0;
	while (y < (int)grid->h)
	{
		x = 0;
		while (x < (int)grid->w)
		{
			if (ft_strchr("WESN", grid->raw[y * (int)grid->w + x]))
			{
				*px = x;
				*py = y;
				return ;
			}
			x++;
		}
		y++;
	}
}

