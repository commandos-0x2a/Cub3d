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
		path = map->north;
	else if (ft_strcmp(dir, "SO ") == 0)
		path = map->south;
	else if (ft_strcmp(dir, "WE ") == 0)
		path = map->west;
	else if (ft_strcmp(dir, "EA ") == 0)
		path = map->east;
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

	*dst = 0;
	color = ft_atoi_r(&line);
	while (*line && isspace(*line))
		line++;
	if (color < 0 || color > 255 || *line != ',')
		return (-1);
	line++; // skip comma
	*dst = color << 16;

	color = ft_atoi_r(&line);
	while (*line && isspace(*line))
		line++;
	if (color < 0 || color > 255 || *line != ',')
		return (-2);
	line++; // skip comma
	*dst |= color << 8;

	color = ft_atoi_r(&line);
	while (*line && isspace(*line))
		line++;
	if (color < 0 || color > 255 || *line != '\0')
		return (-3);
	*dst |= color;
	return (0);
}

int	is_emtpy_line(char *line)
{
	while (*line && isspace(*line))
		line++;
	return (!*line);
}

int	read_grid_iter(int fd, int i, t_map *map, char *line)
{
	int		err;
	size_t	w;
	char	*cur;

	err = 0;
	if (!line)
	{
		map->h = i;
		map->grid = malloc(map->h * map->w);
		if (!map->grid)
			return (-1);
		return (0);
	}
	w = ft_strlen(line);
	if (w > map->w)
		map->w = w;
	err = read_grid_iter(fd, i + 1, map, get_next_line(fd));
	if (err != 0)
		return (err);
	cur = &map->grid[i * map->w];
	while (*line && *line != '\n')
		*cur++ = *line++;
	ft_memset(cur, ' ', &map->grid[(i + 1) * map->w] - cur);
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
	if (ft_strnstr("NO SO WE EA ", dir, -1))
		err = copy_file_path(map, line + 3, dir);
	else if (ft_strncmp(line, "F ", 2) == 0)
		err = copy_color(&map->floor_color, line + 2);
	else if (ft_strncmp(line, "C ", 2) == 0)
		err = copy_color(&map->ceiling_color, line + 2);
	else if (is_emtpy_line(line))
		printf("%i: emtpy line\n", i + 1);
	else
		err = read_grid_iter(fd, 0, map, line);
	if (err != 0)
		return (err);
	err = read_map_iter(fd, i + 1, map);
	return (err);
}

void print_read_error(t_map *map, int err)
{
	(void)map;
	printf("error: %d\n", err);
}

t_map	*init_map()
{
	t_map	*map;

	map = malloc(sizeof(*map));
	if (!map)
		return (NULL);
	map->north[0] = 0;
	map->south[0] = 0;
	map->west[0] = 0;
	map->east[0] = 0;
	map->floor_color = 0;
	map->ceiling_color = 0;
	map->w = 0;
	map->h = 0;
	map->grid = NULL;
	return (map);
}

int	valid_file_name(const char *map_file)
{
	char	*ext;

	ext = ft_strrchr(map_file, '.');
	if (!ext)
		return (0);
	return (ft_strcmp(ext, ".cub") == 0);
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
