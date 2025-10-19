/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:23:37 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/19 15:33:52 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"
#include <stdlib.h>
#include <libft.h>

static void	init_player(t_map *map, t_player *player, t_debug *debug)
{
	player->speed = 4;
	player->r = 0;
	player->pos.x = map->player_x + 0.5f;
	player->pos.y = map->player_y + 0.5f;
	if (map->player_r == 'W')
		player->r = 0.f * PI / 180.f;
	else if (map->player_r == 'N')
		player->r = 90.f * PI / 180.f;
	else if (map->player_r == 'E')
		player->r = 180.f * PI / 180.f;
	else if (map->player_r == 'S')
		player->r = 270.f * PI / 180.f;
	debug->sec = 0;
	debug->fps = 0;
}

int	start_game(t_game *game, const char *map_path)
{
	ft_bzero(game, sizeof(*game));
	game->map = read_map(map_path);
	if (!game->map)
		return (1);
	if (!validate_map(game->map))
		return (1);
	game->width = WIDTH;
	game->height = HEIGHT;
	game->interact = false;
	if (load_textures(game) != 0)
		return (1);
	init_player(game->map, &game->player, &game->debug);
	return (0);
}

int	set_game_icon(t_game *game)
{
	game->texture[ICON_INDEX] = load_texture(game,
			"textures/directions.xpm42", ICON_INDEX);
	if (!game->texture[ICON_INDEX])
		return (1);
	mlx_set_icon(game->mlx, game->texture[ICON_INDEX]);
	return (0);
}

int32_t	open_window(t_game *game)
{
	game->mlx = mlx_init(WIDTH, HEIGHT, "TOO LATE", true);
	if (!game->mlx)
		return (-1);
	game->frame = mlx_new_image(game->mlx, game->width, game->height);
	if (!game->frame)
		return (-1);
	if (mlx_image_to_window(game->mlx, game->frame, 0, 0) == -1)
		return (-1);
	game->last_render = mlx_get_time();
	if (set_game_icon(game) != 0)
		return (-1);
	return (0);
}
