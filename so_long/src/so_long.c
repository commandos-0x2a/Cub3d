/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 23:15:36 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/25 08:09:28 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <MLX42/MLX42.h>

void	end_program(void *_game, int exit_status)
{
	t_so_long1	*game;

	game = (t_so_long1 *)_game;
	if (game->gs)
		destroy_object((void **)&game->gs);
	if (exit_status == 0)
		printf("So_long Bye :)\n");
	else
		printf("Error :(\n");
}

int	is_surround_boat(t_so_long1 *game)
{
	t_object	*boat;
	t_object	*player;
	t_point		b_location;
	t_point		p_location;

	boat = get_children_by_name(&game->gs->components, "boat");
	if (!boat)
		return (0);
	player = get_children_by_name(&game->gs->components, "player");
	if (!player)
		return (0);
	p_location = player->relative_location;
	b_location = boat->relative_location;
	if (pow(p_location.x - b_location.x, 2) \
		+ pow(p_location.y - b_location.y, 2) < 100 * 100)
		return (1);
	return (0);
}

void	ride_boat(t_so_long1 *game)
{
	t_player	*player;
	t_boat		*boat;

	if (!is_surround_boat(game))
		return ;
	player = game->player;
	boat = (t_boat *)get_children_by_name(&game->gs->components, "boat");
	if (!boat)
		return ;
	((t_object *)player)->relative_location = \
			((t_object *)boat)->relative_location;
	player->move_lock = 1;
	boat->move_lock = 0;
	((t_object *)player)->parent_location = &boat->spr.obj.absolute_location;
	((t_object *)player)->relative_location = (t_point){0, 0};
}

int	game_init(t_so_long1 *game, const char *map_path, int width, int height)
{
	game->gs = init_game_schema();
	if (game->gs == NULL)
		return (-1);
	if (open_map_and_check(&game->gs->map, map_path) != 0)
		return (-1);
	game->width = width;
	game->height = height;

	game->frame.tex.width = game->width;
	game->frame.tex.height = game->height;
	game->frame.tex.bytes_per_pixel = sizeof(int);
	game->frame.tex.pixels = malloc(game->width * game->height * sizeof(int));
	return (0);
}

t_so_long1	*init_so_long1(const char *map_path, int width, int height)
{
	t_so_long1	*so;
	t_boat		*boat;
	
	so = calloc(1, sizeof(*so));
	if(!so)
		return (NULL);
	if (game_init(so, map_path, width, height) != 0)
	{
		end_program(so, 1);
		return (NULL);
	}
	
	if (load_schema(so->gs) != 0)
	{
		end_program(so, 1);
		return (NULL);
	}
	
	so->player = (void *)get_children_by_name(&so->gs->components, "player");
	boat = (t_boat *)get_children_by_name(&so->gs->components, "boat");
	boat->game = so;
	
	// mlx_hook(game->win_ptr, KeyRelease, KeyReleaseMask, key_release, &game);
	// mlx_hook(game->win_ptr, KeyPress, KeyPressMask, key_press, &game);
	// mlx_hook(game->win_ptr, DestroyNotify, 0, cross_button, &game);
	so->last_rander = 0;
	so->time = 0;
	// mlx_loop_hook(game->mlx_ptr, rander, &game);
	// mlx_loop(game->mlx_ptr);
	return (so);
}

