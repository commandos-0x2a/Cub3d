/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation_boat_path.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 18:48:35 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/24 17:45:54 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"
#include "ft_printf.h"

static int	get_boat_location(t_grid *p_grid, uint32_t *r, uint32_t *c)
{
	*r = 0;
	while (*r < p_grid->rows)
	{
		*c = 0;
		while (*c < p_grid->cols)
		{
			if (p_grid->blocks[*r][*c] == 'E')
				return (0);
			++*c;
		}
		++*r;
	}
	return (-1);
}

int	test_flood_fill_boat(t_grid *p_grid2, uint32_t r, uint32_t c)
{
	if (r < 0 || r >= p_grid2->rows || c < 0 || c >= p_grid2->cols)
		return (1);
	if (p_grid2->blocks[r][c] != '1' && p_grid2->blocks[r][c] != 'E')
		return (0);
	p_grid2->blocks[r][c] = 'W';
	if (test_flood_fill_boat(p_grid2, r - 1, c))
		return (1);
	if (test_flood_fill_boat(p_grid2, r + 1, c))
		return (1);
	if (test_flood_fill_boat(p_grid2, r, c + 1))
		return (1);
	if (test_flood_fill_boat(p_grid2, r, c - 1))
		return (1);
	return (0);
}

static void	print_error_grid(t_grid *p_grid2, t_grid *o_grid)
{
	uint32_t		r;
	uint32_t		c;
	char	*color;

	r = -1;
	while (++r < o_grid->rows)
	{
		c = -1;
		while (++c < o_grid->cols)
		{
			color = "\e[0;39m";
			if (p_grid2->blocks[r][c] == 'W')
				color = "\e[0;91m";
			ft_fprintf(2, "%s%c\e[0;39m", color, o_grid->blocks[r][c]);
		}
		ft_fprintf(2, "\n");
	}
}

int	print_boat_path_error(t_grid *o_grid, t_grid *p_grid2)
{
	print_error_grid(p_grid2, o_grid);
	ft_fprintf(2, "\n\e[0;91mThe boat can't reach the ocean\e[0;39m\n\n");
	return (-1);
}

int	check_boat_path(t_grid *o_grid)
{
	uint32_t		r;
	uint32_t		c;
	t_grid	p_grid2;

	if (copy_grid(&p_grid2, o_grid) != 0)
		return (-1);
	r = 0;
	c = 0;
	get_boat_location(&p_grid2, &r, &c);
	if (test_flood_fill_boat(&p_grid2, r, c))
	{
		free_grid(&p_grid2);
		return (0);
	}
	print_boat_path_error(o_grid, &p_grid2);
	free_grid(&p_grid2);
	return (-1);
}
