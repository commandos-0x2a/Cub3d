/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 14:20:22 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/15 12:14:56 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"
#include "minimap.h"
#include "libft.h"

void	setup_minimap(t_minimap *minimap, t_map *map)
{
	minimap->x = 150 + 10;
	minimap->y = 150 + 10;
	minimap->r = 0;
	minimap->ref_x = 1;
	minimap->ref_y = 1;
	minimap->width = 300;
	minimap->height = 300;
	(void)map;
	get_matrix(minimap, minimap->mat);
}

void	minimap(t_game *game)
{
	t_minimap	minimap;

	ft_bzero(&minimap, sizeof(minimap));
	setup_minimap(&minimap, game->map);
	draw_minimap(game->frame, &minimap, &game->player, &game->map->grid);
}
