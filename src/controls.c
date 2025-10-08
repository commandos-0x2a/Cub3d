/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:43:36 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/01 21:15:29 by yaltayeh         ###   ########.fr       */
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

void	player_walk(t_game *game, t_player *player, t_vector *vec)
{
	if (mlx_is_key_down(game->mlx, MLX_KEY_W))
	{
		vec->x += cosf(player->r);
		vec->y += sinf(player->r);
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_S))
	{
		vec->x -= cosf(player->r);
		vec->y -= sinf(player->r);
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_A))
	{
		vec->x += sinf(player->r);
		vec->y -= cosf(player->r);
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_D))
	{
		vec->x -= sinf(player->r);
		vec->y += cosf(player->r);
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
		player->r -= 0.001;
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
		player->r += 0.001;
	game->interact = mlx_is_key_down(game->mlx, MLX_KEY_F);
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
	if (new_x != old_x
		&& (game->map->grid.raw[old_y * game->map->grid.w + new_x] == '1'
		|| game->map->grid.raw[old_y * game->map->grid.w + new_x] == 'D'))
		vec->x = 0;
	if (new_y != old_y
		&& (game->map->grid.raw[new_y * game->map->grid.w + old_x] == '1'
		|| game->map->grid.raw[new_y * game->map->grid.w + old_x] == 'D'))
		vec->y = 0;
}

void	handle_door(t_game *game)
{
	int	door_x;
	int	door_y;
	int	w;
	int	h;

	w = game->map->grid.w;
	h = game->map->grid.h;
	door_x = (int)(game->player.pos.x + cosf(game->player.r) * 1.5);
	door_y = (int)(game->player.pos.y + sinf(game->player.r) * 1.5);
	if (door_x < 0 || door_x >= w
		|| door_y < 0 || door_y >= h)
		return ;
	if (game->map->grid.raw[door_y * game->map->grid.w + door_x] == 'D')
	{
		game->map->grid.raw[door_y * w + door_x] = 'O';
		return ;
	}
	if (game->map->grid.raw[door_y * w + door_x] == 'O')
	{
		game->map->grid.raw[door_y * w + door_x] = 'D';
		return ;
	}
}

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
	if(game->interact)
	{
		game->interact = false;
		handle_door(game);
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_Q))
		player->r -= 0.054;
	if (mlx_is_key_down(game->mlx, MLX_KEY_E))
		player->r += 0.054;
	vec.x *= player->speed * game->time_delta;
	vec.y *= player->speed * game->time_delta;
	wall_collision(game, player, &vec);
	player->pos.x += vec.x;
	player->pos.y += vec.y;
}
