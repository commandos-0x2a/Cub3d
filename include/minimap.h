/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 03:13:58 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/15 12:40:40 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIMAP_H
# define MINIMAP_H

# define DRAWING_SCALE 50

# include "utils.h"
# include "game.h"

typedef struct s_minimap
{
	float		x;
	float		y;

	float		r;

	float		ref_x;
	float		ref_y;

	int			width;
	int			height;
	float		mat[3][3];
}	t_minimap;

void	minimap(t_game *game);

void	draw_minimap(mlx_image_t *frame, t_minimap *minimap,
			t_player *player, t_grid *grid);
void	get_matrix(t_minimap *minimap, float mat[3][3]);
t_point	transform_point(float mat[3][3], t_point point);

#endif
