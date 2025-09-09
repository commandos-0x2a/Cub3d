
#ifndef GAME_H
# define GAME_H

# include "map.h"
# include <MLX42/MLX42.h>

# define WIDTH 1080
# define HEIGHT 720

typedef struct s_player
{
	int	x;
	int	y;
	int	r;
}	t_player;

typedef struct s_game
{
	t_map		*map;
	mlx_t		*mlx;
	mlx_image_t	*frame;
	t_player	*player;
}	t_game;

#endif
