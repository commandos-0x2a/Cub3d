#include "map.h"
#include "get_next_line/get_next_line.h"
#include <strings.h>
#include <fcntl.h>
#include <ctype.h>

size_t	strlcpy(char *dst, const char *src, size_t size);

int	copy_file_path(char dst[PATH_MAX], char *line)
{
	if (*dst)
		return (-1);
	while (isspace(*line))
		line++;
	if (!*line)
		return (-1);
	if (strlcpy(dst, line, PATH_MAX) >= PATH_MAX)
		return (-1);
	return (0);
}

int	copy_color(int *dst, char *line)
{
	int	color;

	*dst = 0;
	color = atoi(line);
	*dst = color << 16;

	line = strchr(line, ',');
	if (!line)
		return (-1);
	color = atoi(line);
	*dst |= color << 8;

	line = strchr(line, ',');
	if (!line)
		return (-1);
	color = atoi(line);
	*dst |= color;
	while (!isspace(*line))
		line++;
	return (0);
}

int	read_map_iter(int fd, int i, t_map *map)
{
	char	*line;
	int		err;

	err = 0;
	line = get_next_line(fd);
	if (!line)
	{
		printf("(%zu, %zu)\n", map->w, map->h);
		map->blocks = malloc(map->h * map->w);
		if (!map->blocks)
			return (-1);
		return (0);
	}
	if (strncmp(line, "NO ", 3) == 0)
		err = copy_file_path(map->north, line + 3);
	else if (strncmp(line, "SO ", 3) == 0)
		err = copy_file_path(map->south, line + 3);
	else if (strncmp(line, "WE ", 3) == 0)
		err = copy_file_path(map->west, line + 3);
	else if (strncmp(line, "EA ", 3) == 0)
		err = copy_file_path(map->east, line + 3);
	else if (strncmp(line, "F ", 2) == 0)
		err = copy_color(&map->floor_color, line + 2);
	else if (strncmp(line, "C ", 2) == 0)
		err = copy_color(&map->ceiling_color, line + 2);
	else
		printf("line[%i]: %s", i, line);
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

t_map	*read_map(const char *map_file)
{
	t_map	*map;
	int		fd;
	int		err;

	map = calloc(1, sizeof(*map));
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
