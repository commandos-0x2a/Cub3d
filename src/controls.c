/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controls.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 15:43:36 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/13 17:40:32 by yaltayeh         ###   ########.fr       */
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

void	player_walk(t_game *game, t_player *p, t_vector *vec)
{
	if (mlx_is_key_down(game->mlx, MLX_KEY_W))
		*vec = add_vector(*vec, (t_vector){cosf(p->r), sinf(p->r)});
	if (mlx_is_key_down(game->mlx, MLX_KEY_S))
		*vec = add_vector(*vec, (t_vector){-cosf(p->r), -sinf(p->r)});
	if (mlx_is_key_down(game->mlx, MLX_KEY_A))
		*vec = add_vector(*vec, (t_vector){sinf(p->r), -cosf(p->r)});
	if (mlx_is_key_down(game->mlx, MLX_KEY_D))
		*vec = add_vector(*vec, (t_vector){-sinf(p->r), cosf(p->r)});
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT)
		|| mlx_is_key_down(game->mlx, MLX_KEY_Q))
		p->r -= (180.f * game->time_delta * PI) / 180.f;
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT)
		|| mlx_is_key_down(game->mlx, MLX_KEY_E))
		p->r += (180.f * game->time_delta * PI) / 180.f;
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
	if (!is_in_box(new_x, new_y, game->map->grid.w, game->map->grid.h))
	{
		*vec = (t_vector){0, 0};
		return ;
	}
	if (new_x != old_x
		&& (game->map->grid.raw[old_y * game->map->grid.w + new_x] == '1'
		|| game->map->grid.raw[old_y * game->map->grid.w + new_x] == 'D'))
		vec->x = 0;
	if (new_y != old_y
		&& (game->map->grid.raw[new_y * game->map->grid.w + old_x] == '1'
		|| game->map->grid.raw[new_y * game->map->grid.w + old_x] == 'D'))
		vec->y = 0;
}

void	handle_door(t_player *player, t_grid *grid, double last_render)
{
	int				door_x;
	int				door_y;
	static double	last_use;

	if (last_render - last_use < 0.5)
		return ;
	door_x = (int)(player->pos.x + cosf(player->r));
	door_y = (int)(player->pos.y + sinf(player->r));
	if (!is_in_circle(door_x, door_y, 100))
		return ;
	if (grid->raw[door_y * grid->w + door_x] == 'D')
		grid->raw[door_y * grid->w + door_x] = 'O';
	else if (grid->raw[door_y * grid->w + door_x] == 'O')
		grid->raw[door_y * grid->w + door_x] = 'D';
	last_use = last_render;
}

void	player_control(void *param)
{
	t_game		*game;
	t_player	*player;
	t_vector	vec;

	game = param;
	if (!game || !game->mlx)
		return ;
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		end_game(game, 0);
	player = &game->player;
	vec = (t_vector){0, 0};
	// mouse_control(game, player);
	player_walk(game, player, &vec);
	if(game->interact)
	{
		game->interact = false;
		handle_door(&game->player, &game->map->grid, game->last_render);
	}
	vec.x *= player->speed * game->time_delta;
	vec.y *= player->speed * game->time_delta;
	wall_collision(game, player, &vec);
	player->pos.x += vec.x;
	player->pos.y += vec.y;
}
