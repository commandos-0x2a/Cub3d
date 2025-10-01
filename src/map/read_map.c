/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 20:38:43 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/01 20:41:35 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"
#include "get_next_line.h"
#include <strings.h>
#include <fcntl.h>
#include <ctype.h>
#include "libft.h"
#include <stdio.h>

int	copy_file_path(t_map *map, char *line, char *dir)
{
	char	*path;
	size_t	len;

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
		return (-1);
	if (*path)
		return (-1);
	while (isspace(*line))
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

int	copy_color(int *dst, char *line)
{
	int	color;

	*dst = 0xff;
	color = ft_atoi_r(&line);
	while (*line && isspace(*line))
		line++;
	if (color < 0 || color > 255 || *line != ',')
		return (-1);
	line++;
	*dst |= color << 24;
	color = ft_atoi_r(&line);
	while (*line && isspace(*line))
		line++;
	if (color < 0 || color > 255 || *line != ',')
		return (-2);
	line++;
	*dst |= color << 16;
	color = ft_atoi_r(&line);
	while (*line && isspace(*line))
		line++;
	if (color < 0 || color > 255 || *line != '\0')
		return (-3);
	*dst |= color << 8;
	return (0);
}

int	is_emtpy_line(char *line)
{
	while (*line && isspace(*line))
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

int	read_grid_iter(int fd, int i, t_grid *grid, char *line)
{
	int		err;
	size_t	w;

	err = 0;
	if (!line)
	{
		grid->h = i;
		grid->raw = malloc(grid->h * grid->w);
		if (!grid->raw)
			return (-1);
		return (0);
	}
	w = ft_strlen(line);
	if (w > grid->w)
		grid->w = w;
	err = read_grid_iter(fd, i + 1, grid, get_next_line(fd));
	if (err != 0)
		return (err);
	copy_line(grid, i, line);
	if (i > 0)
		free(line);
	return (0);
}

int	read_map_iter(int fd, int i, t_map *map)
{
	char	*line;
	int		err;
	char	dir[4];

	err = 0;
	line = get_next_line(fd);
	map->first_map_line = i;
	if (!line)
		return (0);
	ft_strlcpy(dir, line, sizeof(dir));
	if (ft_strnstr("NO SO WE EA DO ", dir, -1))
		err = copy_file_path(map, line + 3, dir);
	else if (ft_strncmp(line, "F ", 2) == 0)
		err = copy_color(&map->floor_color, line + 2);
	else if (ft_strncmp(line, "C ", 2) == 0)
		err = copy_color(&map->ceiling_color, line + 2);
	else if (is_emtpy_line(line))
		;
	else
		err = read_grid_iter(fd, 0, &map->grid, line);
	free(line);
	if (err != 0)
		return (err);
	err = read_map_iter(fd, i + 1, map);
	return (err);
}

void	print_read_error(t_map *map, int err)
{
	(void)map;
	printf("error: %d\n", err);
}

t_map	*init_map(void)
{
	t_map	*map;

	map = malloc(sizeof(*map));
	if (!map)
		return (NULL);
	map->north_path[0] = 0;
	map->south_path[0] = 0;
	map->west_path[0] = 0;
	map->east_path[0] = 0;
	map->door_path[0] = 0;
	map->floor_color = 0;
	map->ceiling_color = 0;
	map->grid.w = 0;
	map->grid.h = 0;
	map->grid.raw = NULL;
	return (map);
}

t_map	*read_map(const char *map_file)
{
	t_map	*map;
	int		fd;
	int		err;

	if (!valid_file_name(map_file))
		return (NULL);
	map = init_map();
	if (!map)
		return (NULL);
	fd = open(map_file, O_RDONLY);
	if (fd == -1)
	{
		free(map);
		perror(map_file);
		return (NULL);
	}
	err = read_map_iter(fd, 0, map);
	close(fd);
	if (err != 0)
	{
		print_read_error(map, err);
		free(map);
		return (NULL);
	}
	return (map);
}
