/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boat.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 20:26:08 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/24 17:21:02 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "components.h"
#include "so_long.h"

char	get_mask_value(t_game_schema *gs, t_point new_location);

static int	is_finish_game(t_boat *boat, t_point new_loc)
{
	t_grid	*o_grid;

	o_grid = &((t_so_long1 *)boat->game)->gs->map.o_grid;
	if (new_loc.x <= 0 || new_loc.x >= o_grid->cols * 128 \
		|| new_loc.y <= 0 || new_loc.y >= o_grid->rows * 128)
		return (1);
	return (0);
}

static t_point	get_new_location(t_player *player, t_boat *boat)
{
	t_point	new_location;

	new_location = ((t_object *)boat)->relative_location;
	if (player->direction == FRONT)
		new_location.y += player->speed;
	else if (player->direction == BACK)
		new_location.y -= player->speed;
	else if (player->direction == LEFT)
		new_location.x -= player->speed;
	else if (player->direction == RIGHT)
		new_location.x += player->speed;
	return (new_location);
}

void	update_boat(t_boat *boat)
{
	t_point			old_location;
	t_point			new_location;
	t_player		*player;

	if (boat->move_lock)
		return ;
	player = (t_player *)get_children_by_name(&boat->gs->components, "player");
	if (!player->is_walk)
		return ;
	old_location = ((t_object *)boat)->relative_location;
	new_location = get_new_location(player, boat);
	if (player->direction == FRONT || player->direction == BACK)
		boat->direction = VERTICAL;
	else if (player->direction == RIGHT || player->direction == LEFT)
		boat->direction = HORIZONTAL;
	if (get_mask_value(boat->gs, new_location) == '1')
		((t_object *)boat)->relative_location = new_location;
	if (is_new_movement(old_location, boat->spr.obj.relative_location))
		player->nb_movement += 1;
	if (is_finish_game(boat, new_location))
		end_program(boat->game, 0);
}

void	animate_boat(t_boat *boat)
{
	boat->clip = (t_clip){boat->spr.index * 64, \
							boat->direction * 64, \
								64, 64, 0};
}

t_boat	*init_boat(t_game_schema *gs)
{
	t_boat	*boat;

	boat = malloc(sizeof(t_boat));
	if (!boat)
		return (NULL);
	load_sprites((void *)boat);
	ft_strlcpy((char *)boat, "boat", NAME_SIZE);
	((t_object *)boat)->center_point = (t_point){32, 32};
	((t_object *)boat)->destroy = defult_destroy_object;
	((t_object *)boat)->update = update_boat;
	((t_sprites *)boat)->image = schema_get_image_by_name(gs, "boat");
	boat->clip = (t_clip){0};
	((t_sprites *)boat)->clips = &boat->clip;
	((t_sprites *)boat)->nb_clips = 1;
	((t_sprites *)boat)->index = 0;
	((t_sprites *)boat)->max_index = 4;
	((t_sprites *)boat)->delay = 3;
	((t_sprites *)boat)->animate = animate_boat;
	boat->gs = gs;
	boat->direction = DIAGONAL;
	boat->move_lock = 1;
	boat->is_move = 0;
	return (boat);
}
