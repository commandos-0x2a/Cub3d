/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/11 13:49:11 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/02/01 17:15:51 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"
#include "game_schema.h"

void	update_camera(t_camera	*camera)
{
	t_object		*victom;
	t_game_schema	*gs;
	t_point			map_size;
	t_point			*location;

	gs = (t_game_schema *)camera->schema;
	victom = get_children_by_name(&gs->components, "player");
	if (((t_player *)victom)->move_lock)
		victom = get_children_by_name(&gs->components, "boat");
	location = &camera->obj.relative_location;
	if (victom)
	{
		location->x = -victom->relative_location.x + WIN_WIDTH / 2;
		location->y = -victom->relative_location.y + WIN_HEIGHT / 2;
		if (location->x > 0)
			location->x = 0;
		if (location->y > 0)
			location->y = 0;
		map_size.x = gs->map.x2_grid.cols * 64;
		map_size.y = gs->map.x2_grid.rows * 64;
		if (map_size.x + location->x < WIN_WIDTH)
			location->x = WIN_WIDTH - map_size.x;
		if (map_size.y + location->y < WIN_HEIGHT)
			location->y = WIN_HEIGHT - map_size.y;
	}
}

void	load_camera(t_camera *camera, void *schema)
{
	load_object(camera);
	ft_strlcpy((char *)camera, "camera", NAME_SIZE);
	((t_object *)camera)->update = update_camera;
	camera->schema = schema;
}
