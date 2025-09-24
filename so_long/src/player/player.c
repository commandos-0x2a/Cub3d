/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 11:42:25 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/02/01 01:27:08 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game_schema.h"
#include "math.h"

void	update_player(t_player *player);
void	aminate_player(t_player *player);
void	end_move_player(t_player *player);

static void	player_spr_config(t_player *player, void *gs)
{
	player->spr.image = schema_get_image_by_name(gs, "player");
	((t_sprites *)player)->end_move = end_move_player;
	((t_sprites *)player)->animate = aminate_player;
	((t_sprites *)player)->delay = 1;
	((t_sprites *)player)->index = 0;
	((t_sprites *)player)->max_index = 9;
	((t_sprites *)player)->nb_clips = 1;
	((t_sprites *)player)->clips = &player->clip;
}

t_player	*init_player(void *gs)
{
	t_player	*player;

	player = malloc(sizeof(t_player));
	if (!player)
		return (NULL);
	load_sprites((void *)player);
	ft_strlcpy((char *)player, "player", NAME_SIZE);
	((t_object *)player)->center_point = (t_point){32, 52};
	((t_object *)player)->update = update_player;
	((t_object *)player)->destroy = defult_destroy_object;
	player_spr_config(player, gs);
	player->gs = gs;
	player->clip = (t_clip){0, 0, 64, 64, 1};
	player->logs_count = 0;
	player->is_walk = 0;
	player->touch_component = NULL;
	player->movement = WALK;
	player->direction = FRONT;
	player->speed = PLAYER_SPEED;
	player->nb_movement = 0;
	player->move_lock = 0;
	return (player);
}
