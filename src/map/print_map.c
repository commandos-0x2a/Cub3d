/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 20:41:55 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/01 20:44:02 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"
#include <stdio.h>

void	print_grid(t_grid *grid)
{
	size_t	x;
	size_t	y;

	printf("Map Width: %zu\n", grid->w);
	printf("Map Height: %zu\n", grid->h);
	printf("Map Blocks:\n");
	y = 0;
	while (y < grid->h)
	{
		x = 0;
		while (x < grid->w)
		{
			putchar(grid->raw[y * grid->w + x]);
		}
		putchar('\n');
	}
}

void	print_map(t_map *map)
{
	printf("North Texture: %s\n", map->north_path);
	printf("South Texture: %s\n", map->south_path);
	printf("West Texture: %s\n", map->west_path);
	printf("East Texture: %s\n", map->east_path);
	printf("Door Texture: %s\n", map->door_path);
	printf("Floor Color: %6X\n", map->floor_color);
	printf("Ceiling Color: %6X\n", map->ceiling_color);
	printf("first_map_line: %zu\n", map->first_map_line);
	print_grid(&map->grid);
}
