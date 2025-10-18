/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 03:13:58 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/18 13:47:53 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIMAP_H
# define MINIMAP_H

# define DRAWING_SCALE 32

# define MINIMAP_X		10
# define MINIMAP_Y		10
# define MINIMAP_WIDTH	300
# define MINIMAP_HEIGHT	300
# define GRID_SIZE		0.05f

# define VOID_COLOR			0x000000ff
# define GRID_COLOR			0x000000ff
# define WALL_COLOR			0x00ff00ff
# define SPACE_COLOR		0x000000ff
# define DOOROPEN_COLOR		0xaa5500ff
# define DOORCLOSE_COLOR	0xff00ffff
# define RESPAWN_COLOR		0x000000ff
# define UNKNOW_COLOR		0x000000ff

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
