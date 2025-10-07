/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:22:45 by hassende          #+#    #+#             */
/*   Updated: 2025/10/07 14:23:01 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"

#define MINIMAP_SIZE_FIXED 150

void	clear_minimap_background(t_game *game)
{
	int	i;
	int	j;

	i = 0;
	while (i < MINIMAP_SIZE_FIXED)
	{
		j = 0;
		while (j < MINIMAP_SIZE_FIXED)
		{
			mlx_put_pixel(game->frame, MINIMAP_X + i, MINIMAP_Y + j,
				0x000000FF);
			j++;
		}
		i++;
	}
}
