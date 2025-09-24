/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 05:25:17 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/01/31 22:52:28 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"

int	map_parser(t_map *map, const char *map_path);

void	destroy_map(t_map **map_p)
{
	t_map	*map;

	map = *map_p;
	free_grid(&map->o_grid);
	free_grid(&map->x2_grid);
}

void	animate_map(t_map *map)
{
	int	offset;

	offset = 64 * 3 * map->spr.index;
	map->big_spot = (t_clip){offset + 0, 0, 64, 64, 0};
	map->small_spot = (t_clip){offset + 0, 64, 64, 64, 0};
	map->tileds[0] = (t_clip){offset + 64, 64, 64, 64, 0};
	map->tileds[1] = (t_clip){offset + 64, 0, 64, 64, 0};
	map->tileds[2] = (t_clip){offset + 64 * 2, 0, 64, 64, 0};
	map->tileds[3] = (t_clip){offset + 64 * 2, 64, 64, 64, 0};
	map->tileds[4] = (t_clip){offset, 64 * 4, 64, 64, 0};
	map->tileds[5] = (t_clip){offset, 64 * 2, 64, 64, 0};
	map->tileds[6] = (t_clip){offset + 64 * 2, 64 * 2, 64, 64, 0};
	map->tileds[7] = (t_clip){offset + 64 * 2, 64 * 4, 64, 64, 0};
	map->tileds[8] = (t_clip){offset + 64, 64 * 2, 64, 64, 0};
	map->tileds[9] = (t_clip){offset + 64 * 2, 64 * 3, 64, 64, 0};
	map->tileds[10] = (t_clip){offset + 64, 64 * 4, 64, 64, 0};
	map->tileds[11] = (t_clip){offset, 64 * 3, 64, 64, 0};
	map->tileds[12] = (t_clip){offset + 64, 64 * 3, 64, 64, 0};
	map->tileds[13] = (t_clip){offset, 64 * 5, 64, 64, 0};
	map->tileds[14] = (t_clip){offset + 64, 64 * 5, 64, 64, 0};
	map->tileds[15] = (t_clip){offset + 64 * 2, 64 * 5, 64, 64, 0};
}

int	open_map_and_check(t_map *map, const char *map_path)
{
	size_t	len;

	len = ft_strlen(map_path);
	if (len < 5)
	{
		ft_fprintf(2, "file '%s' doesn't end with .ber\n", map_path);
		return (1);
	}
	if (ft_strcmp(map_path + len - 4, ".ber") != 0)
	{
		ft_fprintf(2, "file '%s' doesn't end with .ber\n", map_path);
		return (1);
	}
	map->nb_collect = map_parser(map, map_path);
	if (map->nb_collect < 1)
		return (1);
	return (0);
}

void	load_map(t_map *map)
{
	ft_bzero(map, sizeof(*map));
	load_sprites(map);
	ft_strlcpy((char *)map, "map", NAME_SIZE);
	((t_object *)map)->destroy = destroy_map;
	((t_sprites *)map)->index = 0;
	((t_sprites *)map)->max_index = 4;
	((t_sprites *)map)->delay = 4;
	((t_sprites *)map)->animate = animate_map;
	map->nb_collect = 0;
}
