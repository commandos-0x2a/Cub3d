/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grid.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 08:01:54 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/24 17:47:58 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRID_H
# define GRID_H

# include "utils.h"

typedef struct s_grid
{
	char	**blocks;
	uint32_t		rows;
	uint32_t		cols;
}	t_grid;

int		scale_x2_grid(t_grid *dst, t_grid *src);
int		copy_grid(t_grid *dst, t_grid *src);
int		free_grid(t_grid *grid);
int		map_normalization(t_grid *o_map, uint32_t rows, uint32_t cols);

#endif