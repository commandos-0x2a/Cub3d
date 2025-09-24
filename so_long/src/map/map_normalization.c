/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_normalization.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 17:00:51 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/24 17:47:47 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "grid.h"

static int	inject_new_value(t_grid *o_grid, t_grid *new_map)
{
	uint32_t	r;

	r = 0;
	while (r < new_map->rows)
	{
		new_map->blocks[r] = ft_calloc(new_map->cols + 1, sizeof(char));
		if (!new_map->blocks[r])
			return (-1);
		ft_memset(new_map->blocks[r], '1', new_map->cols);
		if ((new_map->rows - o_grid->rows) / 2 <= r \
			&& r < (o_grid->rows + (new_map->rows - o_grid->rows) / 2))
		{
			ft_memcpy(new_map->blocks[r] + (new_map->cols - o_grid->cols) / 2, \
				o_grid->blocks[r - (new_map->rows - o_grid->rows) / 2], \
				o_grid->cols);
		}
		r++;
	}
	return (0);
}

int	map_normalization(t_grid *o_grid, uint32_t rows, uint32_t cols)
{
	t_grid	new_map;

	if (o_grid->cols >= cols && o_grid->rows >= rows)
		return (0);
	new_map.cols = o_grid->cols;
	if (o_grid->cols < cols)
		new_map.cols = o_grid->cols + ((cols - o_grid->cols + 1) / 2) * 2;
	new_map.rows = o_grid->rows;
	if (o_grid->rows < rows)
		new_map.rows = o_grid->rows + ((rows - o_grid->rows + 1) / 2) * 2;
	new_map.blocks = ft_calloc(rows + 1, sizeof(char *));
	if (!new_map.blocks)
		return (-1);
	if (inject_new_value(o_grid, &new_map) != 0)
		return (free_grid(&new_map));
	free_grid(o_grid);
	ft_memcpy(o_grid, &new_map, sizeof(t_grid));
	return (0);
}
