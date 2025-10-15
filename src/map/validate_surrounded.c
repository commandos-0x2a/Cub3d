/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_surrounded.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 20:44:11 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/15 12:07:53 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"
#include "libft.h"
#include <stdio.h>

static int	add_block_surrounded(t_stack **stack, t_grid *grid, int x, int y)
{
	if (save_add_to_stack(stack, grid, x, y - 1) < 0
		|| save_add_to_stack(stack, grid, x + 1, y) < 0
		|| save_add_to_stack(stack, grid, x - 1, y) < 0
		|| save_add_to_stack(stack, grid, x, y + 1) < 0)
		return (0);
	return (1);
}

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

int	flood_fill(t_stack **cur_r, t_grid *tmp_grid)
{
	t_stack	*next;
	char	*c;

	while (*cur_r)
	{
		next = (*cur_r)->next;
		c = &tmp_grid->raw[(*cur_r)->y * tmp_grid->w + (*cur_r)->x];
		if (*c == ' '
			|| !is_in_box((*cur_r)->x, (*cur_r)->y, tmp_grid->w, tmp_grid->h))
			return (0);
		if (*c != '1')
		{
			add_block_surrounded(&next, tmp_grid, (*cur_r)->x, (*cur_r)->y);
			*c = '@';
		}
		free(*cur_r);
		*cur_r = next;
	}
	return (1);
}

int	valid_surrounded_wall(t_map *map)
{
	t_stack	*cur;
	t_grid	*tmp_grid;
	int		valid;

	tmp_grid = copy_grid(&map->grid);
	if (!tmp_grid)
		return (0);
	cur = add_player_position(tmp_grid);
	if (!cur)
	{
		free(tmp_grid->raw);
		free(tmp_grid);
		return (0);
	}
	valid = flood_fill(&cur, tmp_grid);
	free(tmp_grid->raw);
	free(tmp_grid);
	clear_stack(&cur);
	return (valid);
}
