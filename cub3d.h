#include <limits.h>
#include <cstddef>

typedef struct	s_map
{
	char	north[PATH_MAX];
	char	south[PATH_MAX];
	char	west[PATH_MAX];
	char	east[PATH_MAX];

	int		floor_color;
	int		ceiling_color;

	size_t	*lens;
	size_t	count;
	char	*blocks;
}	t_map;
