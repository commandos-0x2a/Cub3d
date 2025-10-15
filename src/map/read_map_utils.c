/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_map_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 11:53:42 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/15 12:12:24 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"
#include <stdio.h>
#include "libft.h"

static char	*get_path_dest(t_map *map, char *dir)
{
	char	*path;

	path = NULL;
	if (ft_strcmp(dir, "NO ") == 0)
		path = map->north_path;
	else if (ft_strcmp(dir, "SO ") == 0)
		path = map->south_path;
	else if (ft_strcmp(dir, "WE ") == 0)
		path = map->west_path;
	else if (ft_strcmp(dir, "EA ") == 0)
		path = map->east_path;
	else if (ft_strcmp(dir, "DO ") == 0)
		path = map->door_path;
	else
		printf("WTF\n");
	if (path && *path)
	{
		printf("doublicated '%s' identifier\n", dir);
		return (NULL);
	}
	return (path);
}

int	copy_file_path(t_map *map, char *line, char *dir)
{
	char	*path;
	size_t	len;

	path = get_path_dest(map, dir);
	if (!path)
		return (-1);
	while (ft_isspace(*line))
		line++;
	if (!*line)
		return (-1);
	len = ft_strlcpy(path, line, PATH_MAX);
	if (len >= PATH_MAX)
		return (-1);
	if (path[len - 1] == '\n')
		path[len - 1] = '\0';
	return (0);
}

int	is_emtpy_line(char *line)
{
	while (*line && ft_isspace(*line))
		line++;
	return (!*line);
}

void	copy_line(t_grid *grid, size_t i, char *line)
{
	char	*cur;

	cur = &grid->raw[i * grid->w];
	while (*line && *line != '\n')
		*cur++ = *line++;
	ft_memset(cur, ' ', &grid->raw[(i + 1) * grid->w] - cur);
}
