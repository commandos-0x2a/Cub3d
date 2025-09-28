/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:43:36 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/27 17:40:47 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"
#include <stdio.h>
#include <math.h>

# define INTERACT_RANGE 1.5f

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

void	player_walk(t_game *game, t_player *player, t_vector *vec)
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
	if (mlx_is_key_down(game->mlx, MLX_KEY_F))
		game->interact = true;
	else
		game->interact = false;
}

void wall_collision(t_game *game, t_player *player, t_vector *vec)
{
    float new_x = player->pos.x + vec->x;
    float new_y = player->pos.y + vec->y;

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
}

// handles interaction with doors (at least only doors rn)
void	handle_interaction(t_game *game, t_player *player)
{
	int		tile_w = game->map->grid.w;
	int		tile_h = game->map->grid.h;
	char	*grid = game->map->grid.raw;
    float	interact_x = player->pos.x + cosf(player->r) * INTERACT_RANGE;
    float	interact_y = player->pos.y + sinf(player->r) * INTERACT_RANGE;
	int		interact_tile_x = (int)(interact_x);
	int		interact_tile_y = (int)(interact_y);

    if (!game->interact)
        return;
    if (interact_tile_x >= 0 && interact_tile_x < tile_w &&
        interact_tile_y >= 0 && interact_tile_y < tile_h)
    {
        if (grid[interact_tile_y * tile_w + interact_tile_x] == 'D')
        {
            // Toggle door state (you might want to track door states separately)
            grid[interact_tile_y * tile_w + interact_tile_x] = '0'; // Open door
            // Or implement proper door state management
        }
    }
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
	player_walk(game, player, &vec);

	// The math function uses radians (PI for 180d) instead of normal degress
	if (mlx_is_key_down(game->mlx, MLX_KEY_Q))
		player->r -= 0.054;
	if (mlx_is_key_down(game->mlx, MLX_KEY_E))
		player->r += 0.054;
	// printf("(%.2f, %.2f)\n", vec.x, vec.y);
	vec.x *= player->speed * game->time_delta;
	vec.y *= player->speed * game->time_delta;

	handle_interaction(game, player);
	wall_collision(game, player, &vec);

	player->pos.x += vec.x;
	player->pos.y += vec.y;
}
