/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 22:58:58 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/25 08:13:54 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

// int	key_press(int keycode, t_game *game)
// {
// 	t_player	*p;
// 	int			nb_collect;

// 	p = game->player;
// 	nb_collect = game->gs->map.nb_collect;
// 	if (keycode == KEY_UP \
// 		|| keycode == KEY_DOWN \
// 		|| keycode == KEY_RIGHT \
// 		|| keycode == KEY_LEFT)
// 		player_walk(p, keycode);
// 	if (keycode == KEY_SPACE)
// 	{
// 		if (p->logs_count >= nb_collect)
// 			if (is_surround_boat(game))
// 				ride_boat(game);
// 		player_slash(p);
// 	}
// 	return (0);
// }

// int	key_release(int keycode, t_game *game)
// {
// 	if (keycode == KEY_UP \
// 		|| keycode == KEY_DOWN \
// 		|| keycode == KEY_RIGHT \
// 		|| keycode == KEY_LEFT)
// 		game->player->is_walk = 0;
// 	if (keycode == KEY_SPACE)
// 	{
// 		game->player->movement = WALK;
// 		game->player->spr.max_index = 9;
// 		game->player->spr.index = 0;
// 	}
// 	if (keycode == KEY_ESC)
// 		end_program(game, 0);
// 	return (0);
// }

void	rander_solong1(void *params)
{
	t_image	*frame;
	t_so_long1	*so;

	so = params;
	frame = &so->frame;
	ft_bzero(frame->tex.pixels, frame->tex.width * frame->tex.height);
	update_object(so->gs);
	animate_sprites(so->gs);
	render_object(so->gs, frame, 0);
	render_object(so->gs, frame, 1);
	render_object(so->gs, frame, 2);
}
