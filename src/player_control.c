#include "game.h"

void	player_control(void *param)
{
	t_game		*game;
	t_player	*player;
	
	game = param;
	player = game->player;

	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
	if (mlx_is_key_down(game->mlx, MLX_KEY_UP))
		// image->instances[0].y -= 5;
	if (mlx_is_key_down(game->mlx, MLX_KEY_DOWN))
		// image->instances[0].y += 5;
		;
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
		// image->instances[0].x -= 5;
		;
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
		// image->instances[0].x += 5;
		;
}
