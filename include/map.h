/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 09:11:20 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/06 11:29:19 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	MAP_H
# define MAP_H

# ifdef __linux__
#  include <linux/limits.h>
# else
#  include <limits.h>
# endif

#define RED		"\x1b[31m"
#define GREEN	"\x1b[32m"
#define YELLOW	"\x1b[33m"
#define BLUE	"\x1b[34m"
#define MAGENTA	"\x1b[35m"
#define CYAN	"\x1b[36m"
#define RESET	"\x1b[0m"

typedef unsigned long int	size_t;

typedef struct	s_queue
{
	int				x;
	int				y;
	struct s_queue	*next;
}	t_queue;

typedef struct	s_map
{
	char	north[PATH_MAX];
	char	south[PATH_MAX];
	char	west[PATH_MAX];
	char	east[PATH_MAX];

	int		floor_color;
	int		ceiling_color;

	size_t	first_map_line;
	size_t	w;
	size_t	h;
	char	*grid;
}	t_map;

t_map	*read_map(const char *map_file);
int		validate_map(t_map *map);
int		valid_file_name(const char *map_file);


#endif
