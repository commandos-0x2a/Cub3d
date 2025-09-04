#include <linux/limits.h>

typedef unsigned long int size_t;

typedef struct	s_map
{
	char	north[PATH_MAX];
	char	south[PATH_MAX];
	char	west[PATH_MAX];
	char	east[PATH_MAX];

	int		floor_color;
	int		ceiling_color;

	size_t	w;
	size_t	h;
	char	*blocks;
}	t_map;

t_map	*read_map(const char *map_file);
