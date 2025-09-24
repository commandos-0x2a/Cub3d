/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 08:18:17 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/24 17:44:15 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

int	check_rectangular(t_grid *o_grid)
{
	int	r;

	if (!o_grid->blocks[0])
		return (-1);
	o_grid->cols = ft_strlen(o_grid->blocks[0]);
	r = 0;
	while (o_grid->blocks[r])
	{
		if (o_grid->cols != ft_strlen(o_grid->blocks[r]))
			return (-1);
		r++;
	}
	o_grid->rows = r;
	return (0);
}

static int	check_surrounded_helper(t_grid *o_grid, int r)
{
	int	i;

	i = 0;
	while (o_grid->blocks[r][i] == '1' || o_grid->blocks[r][i] == 'E')
		i++;
	if (o_grid->blocks[r][i] != '\0')
		return (print_error_line(o_grid->blocks, r, i, \
									"map not closed"));
	return (0);
}

int	check_surrounded(t_grid *o_grid)
{
	int	r;

	if (o_grid->blocks[0] && check_surrounded_helper(o_grid, 0) != 0)
		return (-1);
	r = -1;
	while (o_grid->blocks[++r])
	{
		if ((o_grid->blocks[r][0] != '1' && o_grid->blocks[r][0] != 'E') \
				|| (o_grid->blocks[r][o_grid->cols - 1] != '1' \
				&& o_grid->blocks[r][o_grid->cols - 1] != 'E'))
		{
			if (o_grid->blocks[r][0] != '1')
				return (print_error_line(o_grid->blocks, r, 0, \
								"map not closed"));
			return (print_error_line(o_grid->blocks, r, o_grid->cols - 1, \
								"map not closed"));
		}
	}
	if (r > 0 && check_surrounded_helper(o_grid, r - 1) != 0)
		return (-1);
	o_grid->rows = r;
	return (0);
}
