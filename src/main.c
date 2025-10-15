/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 09:28:39 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/15 11:17:28 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "map.h"
#include <stdio.h>
#include <MLX42/MLX42.h>
#include "game.h"
#include "libft.h"

int	main(int argc, char *argv[])
{
	t_game	game;

	if (argc != 2)
	{
		printf("%s map_file\n", argv[0]);
		return (1);
	}
	if (start_game(&game, argv[1]) != 0)
		end_game(&game, 1);
	if (open_window(&game) != 0)
		end_game(&game, 1);
	game_hooks(&game);
	mlx_loop(game.mlx);
	end_game(&game, 0);
	return (0);
}
