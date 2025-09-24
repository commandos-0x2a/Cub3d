/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 00:36:48 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/24 17:32:26 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include <stdio.h>
# include <libft.h>
# include "map.h"
# include "game_schema.h"
# include <math.h>

typedef struct s_game
{
	t_game_schema	*gs;
	int				width;
	int				height;
	t_player		*player;
	t_image			frame;
	t_clock			last_rander;
	t_clock			time;
}	t_so_long1;

int		ft_islower(int c);
int		ft_isupper(int c);

int		is_surround_boat(t_so_long1 *game);
void	ride_boat(t_so_long1 *game);
void	end_program(void *_game, int exit_status);

int		key_press(int keycode, t_so_long1 *game);
int		key_release(int keycode, t_so_long1 *game);
int		rander(t_so_long1 *game);
int		cross_button(t_so_long1 *game);

#endif