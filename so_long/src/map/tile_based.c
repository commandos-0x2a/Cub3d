/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tile_based.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 07:53:04 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/25 10:04:58 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tile.h"

int	render_tile(t_tile *tile, mlx_texture_t *frame, int layer)
{
	t_point	cur;
	t_point	draw_loc;
	int		index;

	render_sprites(tile, frame, layer);
	if (layer != 0)
		return (0);
	cur.y = -1;
	while (++cur.y < 64)
	{
		cur.x = -1;
		while (++cur.x < 64)
		{
			draw_loc = add_point(tile->spr.obj.draw_location, cur);
			if (draw_loc.x < 0 || draw_loc.x >= frame->width
				|| draw_loc.y < 0 || draw_loc.y >= frame->height)
				continue ;
			index = (cur.y / (64 / MASK_SIZE)) * MASK_SIZE \
					+ (cur.x / (64 / MASK_SIZE));
			if (tile->mask[index] == '*')
				frame->pixels[draw_loc.y * frame->width + draw_loc.x] \
									|= 0xf0ff0000;
		}
	}
	return (0);
}

t_tile	*init_tile(t_map *map, void *schema, t_cell cell)
{
	t_tile	*tile;

	tile = malloc(sizeof(*tile));
	if (!tile)
		return (NULL);
	load_sprites((void *)tile);
	ft_strlcpy((char *)tile, "tile", NAME_SIZE);
	((t_object *)tile)->parent_location = \
				&((t_object *)map)->absolute_location;
	((t_object *)tile)->destroy = defult_destroy_object;
	((t_object *)tile)->render = render_sprites;
	((t_sprites *)tile)->image = schema_get_image_by_name(schema, "tile");
	tile->index = get_tile_index(&map->x2_grid, cell);
	tile->mask = get_tile_mask(tile->index);
	tile->cell = cell;
	((t_sprites *)tile)->clips = &map->tileds[tile->index];
	((t_sprites *)tile)->nb_clips = 1;
	return (tile);
}

/*
	((t_object *)tile)->relative_location.x += (cell.c + 1) % 2;
	((t_object *)tile)->relative_location.y += (cell.r + 1) % 2;
*/
int	init_tiles(t_map *map, void *schema)
{
	t_cell	cell;
	t_tile	*tile;

	cell.r = 0;
	while (cell.r < map->x2_grid.rows)
	{
		cell.c = 0;
		while (cell.c < map->x2_grid.cols)
		{
			tile = init_tile(map, schema, cell);
			if (!tile)
				return (-1);
			((t_object *)tile)->relative_location.x = cell.c * 64;
			((t_object *)tile)->relative_location.y = cell.r * 64;
			add_children(map, tile);
			cell.c++;
		}
		cell.r++;
	}
	return (0);
}
