/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 09:11:20 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/19 15:10:55 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_H
# define MAP_H

# ifdef __linux__
#  include <linux/limits.h>
# else
#  include <limits.h>
# endif
# include "utils.h"
# include <stdlib.h>

# define RED		"\x1b[31m"
# define GREEN	"\x1b[32m"
# define YELLOW	"\x1b[33m"
# define BLUE	"\x1b[34m"
# define MAGENTA	"\x1b[35m"
# define CYAN	"\x1b[36m"
# define RESET	"\x1b[0m"

typedef struct s_grid
{
	size_t	w;
	size_t	h;
	char	*raw;
}	t_grid;

typedef struct s_map
{
	char	north_path[PATH_MAX];
	char	south_path[PATH_MAX];
	char	west_path[PATH_MAX];
	char	east_path[PATH_MAX];
	char	door_path[PATH_MAX];
	int		floor_color;
	int		ceiling_color;
	size_t	first_map_line;
	t_grid	grid;
	int		player_x;
	int		player_y;
	char	player_r;	
}	t_map;

t_map	*read_map(const char *map_file);
int		validate_map(t_map *map);
int		valid_file_name(const char *map_file);
int		valid_surrounded_wall(t_map *map);

/*  Utils  */
size_t	grid_linelen(const char *line);
int		copy_file_path(t_map *map, char *line, char *dir);
int		is_emtpy_line(char *line);
void	copy_line(t_grid *grid, size_t i, char *line);
int		copy_color(int *color, char *line);

void	get_player_position(t_grid *grid, int *x, int *y);

#endif
