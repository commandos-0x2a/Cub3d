/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:43:36 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/15 18:58:44 by yaltayeh         ###   ########.fr       */
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

void	wall_collision(t_game *game, t_player *player, t_vector *vec)
{
	int	new_x;
	int	new_y;
	int	old_x;
	int	old_y;
	
	new_x = player->pos.x + vec->x;
	new_y = player->pos.y + vec->y;
	old_x = player->pos.x;
	old_y = player->pos.y;
	
	printf("%+d, %+d --- ", new_x - old_x, new_y - old_y);
	printf("%.2f, %.2f --- ", vec->x, vec->y);
	if (new_x != old_x && game->map->grid.raw[old_y * game->map->grid.w + new_x] == '1')
		vec->x = 0;
	if (new_y != old_y && game->map->grid.raw[new_y * game->map->grid.w + old_x] == '1')
		vec->y = 0;
}

// simple player control
void	player_control(void *param)
{
	t_game		*game;
	t_player	*player;
	t_vector	vec;
	int			new_x;
	int			mapy;

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
	printf("%.2f, %.2f\n", vec.x, vec.y);
	player->pos.x += vec.x;
	player->pos.y += vec.y;
}
