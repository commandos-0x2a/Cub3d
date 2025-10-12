/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 13:23:37 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/12 16:17:21 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "game.h"
#include <stdlib.h>
#include <libft.h>

void	free_textures(t_game *game)
{
	size_t	i;

	i = 0;
	while (i < sizeof(game->texture) / sizeof(game->texture[0]))
	{
		if (game->texture[i])
		{
			if (game->texture[i]->pixels)
				free(game->texture[i]->pixels);
			free(game->texture[i]);
			printf("%s: %zu\n", __func__, i);
			game->texture[i] = NULL;
		}
		i++;
	}
	i = 0;
	while (i < game->nb_animate)
	{
		if (game->animates[i]->spr)
			free_sprite(game->animates[i]->spr);
		i++;
	}
}

void	end_game(t_game *game, int status)
{
	if (mlx_errno != 0)
		puts(mlx_strerror(mlx_errno));
	if (game->mlx)
	{
		// mlx_destroy_cursor(game->mlx);
		// mlx_close_window(game->mlx);
		mlx_terminate(game->mlx);
		game->mlx = NULL;
	}
	free_textures(game);
	if (game->map)
	{
		free(game->map->grid.raw);
		free(game->map);
	}
	free(game);
	(void)status;
	exit(status);
}

static void	init_player(t_map *map, t_player *player, t_debug *debug)
{
	player->speed = 4;
	player->r = 0;
	player->pos.x = map->player_x + 0.5f;
	player->pos.y = map->player_y + 0.5f;
	if (map->player_r == 'E')
		player->r = 0.f * PI / 180.f;
	else if (map->player_r == 'W')
		player->r = 90.f * PI / 180.f;
	else if (map->player_r == 'N')
		player->r = 180.f * PI / 180.f;
	else if (map->player_r == 'S')
		player->r = 270.f * PI / 180.f;
	debug->sec = 0;
	debug->fps = 0;
}

t_game	*start_game(const char *map_path)
{
	t_game	*game;

	game = ft_calloc(1, sizeof(t_game));
	if (!game)
		return (NULL);
	game->map = read_map(map_path);
	if (!game->map)
		end_game(game, 1);
	if (!validate_map(game->map))
		end_game(game, 1);
	game->width = WIDTH;
	game->height = HEIGHT;
	game->interact = false;
	if (load_textures(game) != 0)
		end_game(game, 1);
	init_player(game->map, &game->player, &game->debug);
	return (game);
}

int32_t	open_window(t_game *game)
{
	game->mlx = mlx_init(WIDTH, HEIGHT, "MLX42", true);
	if (!game->mlx)
		return (-1);
	game->frame = mlx_new_image(game->mlx, game->width, game->height);
	if (!game->frame)
		return (-1);
	if (mlx_image_to_window(game->mlx, game->frame, 0, 0) == -1)
		return (-1);
	game->last_render = mlx_get_time();
	return (0);
}
