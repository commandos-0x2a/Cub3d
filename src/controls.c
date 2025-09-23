/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:43:36 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/27 08:24:37 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"
#include <stdio.h>
#include <math.h>

void	mouse_control(t_game *game, t_player *player)
{
	int	x;
	int	y;
	int	dx;

	mlx_get_mouse_pos(game->mlx, &x, &y);

	(void)y;
	dx = x - game->width / 2;

	mlx_set_mouse_pos(game->mlx, game->width / 2, game->height / 2);
	player->r += dx * 0.005f;

}

void	keyboard_control(t_game *game, t_player *player, t_vector *vec)
{
	if (mlx_is_key_down(game->mlx, MLX_KEY_W) || mlx_is_key_down(game->mlx, MLX_KEY_UP)) // front
	{
		vec->x += cosf(player->r);
		vec->y += sinf(player->r);
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_S) || mlx_is_key_down(game->mlx, MLX_KEY_DOWN)) // back
	{
		vec->x -= cosf(player->r);
		vec->y -= sinf(player->r);
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_A) || mlx_is_key_down(game->mlx, MLX_KEY_LEFT)) // left
	{
		vec->x += sinf(player->r);
		vec->y -= cosf(player->r);
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_D) || mlx_is_key_down(game->mlx, MLX_KEY_RIGHT)) // right
	{
		vec->x -= sinf(player->r);
		vec->y += cosf(player->r);
	}
}

#define SAFE_MARGIN 0.35f  // Tune between 0.1 and 0.3

void wall_collision(t_game *game, t_player *player, t_vector *vec)
{
    float new_x = player->pos.x + vec->x;
    float new_y = player->pos.y + vec->y;

<<<<<<< HEAD
	new_x = player->pos.x + vec->x;
	new_y = player->pos.y + vec->y;
	old_x = player->pos.x;
	old_y = player->pos.y;
	
	// printf("%+d, %+d --- ", new_x - old_x, new_y - old_y);
	// printf("%.2f, %.2f --- ", vec->x, vec->y);
	if (new_x != old_x && game->map->grid.raw[old_y * game->map->grid.w + new_x] == '1')
		vec->x = 0;
	if (new_y != old_y && game->map->grid.raw[new_y * game->map->grid.w + old_x] == '1')
		vec->y = 0;
=======
    int tile_w = game->map->grid.w;
    int tile_h = game->map->grid.h;
    char *grid = game->map->grid.raw;

    // check right - left walls
    int left_tile  = (int)(new_x - SAFE_MARGIN);
    int right_tile = (int)(new_x + SAFE_MARGIN);
    int center_y   = (int)new_y;  // we use y to check rows. Since we need X to check left/right walls

    if (center_y >= 0 && center_y < tile_h)
    {
        // Too close to left wall?
        if (left_tile >= 0 && (grid[center_y * tile_w + left_tile] == '1' || grid[center_y * tile_w + left_tile] == 'D'))
            new_x = (float)left_tile + 1.0f + SAFE_MARGIN;

        // Too close to right wall?
        if (right_tile < tile_w && (grid[center_y * tile_w + right_tile] == '1' || grid[center_y * tile_w + right_tile] == 'D'))
            new_x = (float)right_tile - SAFE_MARGIN;
    }

    // check top - bottom walls
    int top_tile    = (int)(new_y - SAFE_MARGIN);
    int bottom_tile = (int)(new_y + SAFE_MARGIN);
    int center_x    = (int)new_x;

    if (center_x >= 0 && center_x < tile_w)
    {
        // Too close to top wall?
        if (top_tile >= 0 && (grid[top_tile * tile_w + center_x] == '1' || grid[top_tile * tile_w + center_x] == 'D'))
            new_y = (float)top_tile + 1.0f + SAFE_MARGIN;

        // Too close to bottom wall?
        if (bottom_tile < tile_h && (grid[bottom_tile * tile_w + center_x] == '1' || grid[bottom_tile * tile_w + center_x] == 'D'))
            new_y = (float)bottom_tile - SAFE_MARGIN;
    }

    // clamp
    vec->x = new_x - player->pos.x;
    vec->y = new_y - player->pos.y;
>>>>>>> 517120e (Fixed near-wall collision and added door texture | moving to adding its logic)
}

// simple player control
void	player_control(void *param)
{
	t_game		*game;
	t_player	*player;
	t_vector	vec;

	game = param;
	player = &game->player;

	vec = (t_vector){0, 0};
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);

	mouse_control(game, player);
	keyboard_control(game, player, &vec);

	// The math function uses radians (PI for 180d) instead of normal degress
	if (mlx_is_key_down(game->mlx, MLX_KEY_Q))
		player->r -= 0.054;
	if (mlx_is_key_down(game->mlx, MLX_KEY_E))
		player->r += 0.054;
	// printf("(%.2f, %.2f)\n", vec.x, vec.y);
	vec.x *= player->speed * game->time_delta;
	vec.y *= player->speed * game->time_delta;

	wall_collision(game, player, &vec);

	// float magnitude = sqrtf(vec.x * vec.x + vec.y * vec.y);
	// if (magnitude > 0)
	// {
	// 	vec.x /= magnitude;
	// 	vec.y /= magnitude;
	// }
	// printf("%.2f, %.2f\n", vec.x, vec.y);
	player->pos.x += vec.x;
	player->pos.y += vec.y;
}
