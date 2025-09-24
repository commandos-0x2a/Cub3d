/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 20:57:34 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/24 17:44:42 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_H
# define MAP_H

# include "sprites.h"
# include "schema.h"
# include "config.h"
# include "grid.h"
# include "tile.h"

typedef struct s_map
{
	t_sprites	spr;
	t_clip		tileds[16];
	t_clip		big_spot;
	t_clip		small_spot;
	t_grid		o_grid;
	t_grid		x2_grid;
	int			nb_collect;
}	t_map;

void	load_map(t_map *map);
int		open_map_and_check(t_map *map, const char *map_path);

int		print_error_line(char **lines, int line_no, int char_no, char *msg);
int		print_error_number1(t_grid *grid, int *nb);
int		print_error_number2(t_grid *p_grid, t_grid *o_grid, int *nb);

int		check_path(t_grid *o_grid);
int		valid_characters(t_grid *grid, int *nb);
void	test_flood_fill(t_grid *p_grid, int *nb, uint32_t r, uint32_t c);
int		check_surrounded(t_grid *o_grid);
int		check_rectangular(t_grid *o_grid);
int		check_boat_path(t_grid *o_grid);

#endif
