/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grid_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 21:23:16 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/24 17:46:55 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	copy_grid(t_grid *dst, t_grid *src)
{
	uint32_t	r;

	dst->rows = src->rows;
	dst->cols = src->cols;
	dst->blocks = ft_calloc(dst->rows, sizeof(char *));
	if (!dst->blocks)
		return (-1);
	r = 0;
	while (r < dst->rows)
	{
		dst->blocks[r] = ft_strdup(src->blocks[r]);
		if (!dst->blocks[r])
			return (-1);
		r++;
	}
	return (0);
}

int	free_grid(t_grid *grid)
{
	uint32_t		i;

	i = 0;
	if (grid->blocks)
	{
		while (i < grid->rows && grid->blocks[i])
		{
			free(grid->blocks[i]);
			grid->blocks[i] = NULL;
			i++;
		}
		free(grid->blocks);
		grid->blocks = NULL;
	}
	grid->rows = 0;
	grid->cols = 0;
	return (-1);
}

int	scale_x2_grid(t_grid *dst, t_grid *src)
{
	uint32_t		r;
	uint32_t		c;

	dst->rows = src->rows * 2;
	dst->cols = src->cols * 2;
	dst->blocks = ft_calloc(dst->rows, sizeof(char *));
	if (!dst->blocks)
		return (-1);
	r = -1;
	while (++r < dst->rows)
	{
		dst->blocks[r] = malloc(dst->cols);
		if (!dst->blocks[r])
			return (-1);
		c = -1;
		while (++c < dst->cols)
		{
			if (src->blocks[r / 2][c / 2] == '1' \
				|| src->blocks[r / 2][c / 2] == 'E')
				dst->blocks[r][c] = '1';
			else
				dst->blocks[r][c] = '0';
		}
	}
	return (0);
}

int	get_player_location(t_grid *p_grid, uint32_t *r, uint32_t *c)
{
	*r = 0;
	while (*r < p_grid->rows)
	{
		*c = 0;
		while (*c < p_grid->cols)
		{
			if (p_grid->blocks[*r][*c] == 'P')
				return (0);
			++*c;
		}
		++*r;
	}
	return (-1);
}
