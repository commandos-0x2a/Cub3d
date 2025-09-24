/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tile.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 08:24:29 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/02/02 11:46:55 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TILE_H
# define TILE_H

# include "map.h"
# include "grid.h"

# define MASK_SIZE 8

typedef struct s_map	t_map;

typedef struct s_tile
{
	t_sprites	spr;
	int			index;
	const char	*mask;
	t_cell		cell;
}	t_tile;

int			init_tiles(t_map *map, void *schema);
int			get_tile_index(t_grid *x2_grid, t_cell cell);
const char	*get_tile_mask(int index);

#endif