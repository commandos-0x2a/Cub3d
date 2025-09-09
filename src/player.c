#include "game.h"

void update_player_pos(t_game *game)
{
    mlx_put_pixel(game->frame, (WIDTH / 2 + game->player.pos.x) * 10, (HEIGHT / 2 + game->player.pos.y) * 10, 0);
}