/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fire.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 20:01:22 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/01/30 10:02:00 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "components.h"

static void	animate_fire(t_fire *fire)
{
	fire->clips[0] = (t_clip){fire->spr.index * 132, 92, 132, 92, 0};
	fire->clips[1] = (t_clip){fire->spr.index * 132, 0, 132, 92, 1};
}

t_fire	*init_fire(t_game_schema *gs)
{
	t_fire	*fire;

	fire = malloc(sizeof(t_fire));
	if (!fire)
		return (NULL);
	load_sprites(fire);
	ft_strlcpy((char *)fire, "fire", NAME_SIZE);
	((t_object *)fire)->center_point = (t_point){66, 46};
	((t_object *)fire)->destroy = defult_destroy_object;
	((t_sprites *)fire)->image = schema_get_image_by_name(gs, "fire");
	((t_sprites *)fire)->index = 0;
	((t_sprites *)fire)->max_index = 6;
	((t_sprites *)fire)->clips = fire->clips;
	((t_sprites *)fire)->nb_clips = 2;
	((t_sprites *)fire)->delay = 2;
	((t_sprites *)fire)->animate = animate_fire;
	return (fire);
}
