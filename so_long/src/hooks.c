/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 22:58:58 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/25 10:36:33 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	solong1_key_hook(void *params)
{
	t_so_long1	*so;
	t_player	*p;
	int			nb_collect;

	so = params;

	p = so->player;
	nb_collect = so->gs->map.nb_collect;
	player_walk(p, so->mlx);
}

void	rander_solong1(void *params)
{
	mlx_texture_t	*frame;
	t_so_long1	*so;

	so = params;
	frame = &so->frame;
	ft_bzero(frame->pixels, frame->width * frame->height);
	update_object(so->gs);
	animate_sprites(so->gs);
	render_object(so->gs, frame, 0);
	render_object(so->gs, frame, 1);
	render_object(so->gs, frame, 2);
}
