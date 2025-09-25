/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 13:13:19 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/02/02 01:35:26 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PLAYER_H
# define PLAYER_H

# include "sprites.h"

enum e_directions
{
	BACK,
	LEFT,
	FRONT,
	RIGHT
};

enum e_move_type
{
	SPELLCAST,
	THRUST,
	WALK,
	SLASH_128
};

typedef struct s_player
{
	t_sprites			spr;
	t_clip				clip;
	int					logs_count;
	enum e_move_type	movement;
	enum e_directions	direction;
	int					is_walk;
	int					speed;
	void				*gs;
	t_object			*touch_component;
	int					nb_movement;
	int					move_lock;
}	t_player;

t_player	*init_player(void *game_schema);
void		player_slash(t_player *player);
void		player_walk(t_player *player, int keycode);

#endif