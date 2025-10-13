/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end_game.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 14:10:22 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/13 17:45:48 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"
#include <stdlib.h>
#include <libft.h>

void	free_animate(t_sprite_animate *animate)
{
	if (!animate)
		return ;
	if (animate->spr)
		free_sprite(animate->spr);
	free(animate);
}

void	free_textures(t_game *game)
{
	size_t	i;

	i = 0;
	while (i < sizeof(game->texture) / sizeof(game->texture[0]))
	{
		if (game->texture[i])
		{
			if (game->is_animate & (1 << i))
				free_animate(game->animates[i]);
			else
				mlx_delete_texture(game->texture[i]);
			game->texture[i] = NULL;
		}
		i++;
	}
}

void	end_game(t_game *game, int status)
{
	if (mlx_errno != 0)
		puts(mlx_strerror(mlx_errno));
	if (game->mlx)
	{
		// mlx_destroy_cursor(game->mlx);
		mlx_close_window(game->mlx);
		mlx_terminate(game->mlx);
		// game->mlx = NULL;
	}
	free_textures(game);
	if (game->map)
	{
		free(game->map->grid.raw);
		free(game->map);
		game->map = NULL;
	}
	(void)status;
	exit(status);
}
