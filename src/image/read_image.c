/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_image.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 20:25:27 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/02 12:53:09 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include <stdio.h>
#include "libft.h"
#include "game.h"
#include "MLX42/MLX42.h"
#include "sprite.h"

t_image_type	get_image_type(const char *path)
{
	char	*ext;

	ext = ft_strrchr(path, '.');
	if (!ext)
		return (IT_NONE);
	else if (ft_strcmp(ext, ".png") == 0)
		return (IT_PNG);
	else if (ft_strcmp(ext, ".xpm") == 0)
		return (IT_XPM);
	else if (ft_strcmp(ext, ".spr") == 0)
		return (IT_SRP);
	return (IT_NONE);
}

static void	print_ext_not_exist(const char *path)
{
	const char	*ext;

	ext = ft_strrchr(path, '.');
	if (!ext)
		printf("Error: cannot load '%s' ext not exist.\n", path);
	else
		printf("Error: cannot load '%s' ext '%s' not exist.\n", path, ext);
}

void	print_tex_loade_error(const char *path, t_image_type type)
{
	if (type == IT_PNG || type == IT_XPM)
		printf("Error: load image (%s): %s\n", path, mlx_strerror(mlx_errno));
}

static mlx_texture_t	*load_texture(t_game *game, const char *path)
{
	t_image_type	type;
	mlx_texture_t	*tex;

	type = get_image_type(path);
	if (type == IT_PNG)
		tex = mlx_load_png(path);
	else if (type == IT_XPM)
		tex = (void *)mlx_load_xpm42(path);
	else if (type == IT_SRP)
	{
		tex = (void *)load_animate_sprite(path, 0);
		if (tex)
		{
			game->animates[game->nb_animate] = (void *)tex;
			game->nb_animate++;
		}
	}
	else
	{
		print_ext_not_exist(path);
		return (NULL);
	}
	if (!tex)
		print_tex_loade_error(path, type);
	return (tex);
}

int	load_textures(t_game *game)
{
	t_map	*map;

	game->nb_animate = 0;
	map = game->map;
	if (!map->west_path[0] || !map->south_path[0] || !map->north_path[0]
		|| !map->east_path[0] || !map->door_path[0])
		return (-1);
	game->texture[WALL_EAST] = load_texture(game, map->east_path);
	if (!game->texture[WALL_EAST])
		return (-1);
	game->texture[WALL_WEST] = load_texture(game, map->west_path);
	if (!game->texture[WALL_WEST])
		return (-1);
	game->texture[WALL_NORTH] = load_texture(game, map->north_path);
	if (!game->texture[WALL_NORTH])
		return (-1);
	game->texture[WALL_SOUTH] = load_texture(game, map->south_path);
	if (!game->texture[WALL_SOUTH])
		return (-1);
	game->texture[WALL_DOOR] = load_texture(game, map->door_path);
	if (!game->texture[WALL_DOOR])
		return (-1);
	return (0);
}
