/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_surrounded.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 20:44:11 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/19 15:24:23 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"
#include "libft.h"
#include <stdio.h>

static t_grid	*copy_grid(t_grid *grid)
{
	t_grid	*new_grid;

	new_grid = malloc(sizeof(*new_grid));
	if (!new_grid)
		return (NULL);
	new_grid->w = grid->w;
	new_grid->h = grid->h;
	new_grid->raw = malloc(grid->w * grid->h);
	ft_memcpy(new_grid->raw, grid->raw, grid->w * grid->h);
	return (new_grid);
}

int	flood_fill(t_grid *tmp_grid, int x, int y)
{
	char	*c;

	if (!is_in_box(x, y, tmp_grid->w, tmp_grid->h))
		return (0);
	c = &tmp_grid->raw[y * tmp_grid->w + x];
	if (*c == ' ')
		return (0);
	if (*c == '@')
		return (1);
	if (*c != '1')
	{
		*c = '@';
		if (!flood_fill(tmp_grid, x, y - 1))
			return (0);
		if (!flood_fill(tmp_grid, x - 1, y))
			return (0);
		if (!flood_fill(tmp_grid, x + 1, y))
			return (0);
		if (!flood_fill(tmp_grid, x, y + 1))
			return (0);
	}
	return (1);
}

int	valid_surrounded_wall(t_map *map)
{
	t_grid	*tmp_grid;
	int		valid;
	int		px;
	int		py;

	tmp_grid = copy_grid(&map->grid);
	if (!tmp_grid)
		return (0);
	get_player_position(tmp_grid, &px, &py);
	valid = flood_fill(tmp_grid, px, py);
	free(tmp_grid->raw);
	free(tmp_grid);
	return (valid);
}
