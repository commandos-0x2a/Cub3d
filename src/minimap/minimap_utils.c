/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 17:45:42 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/15 11:20:01 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minimap.h"
#include "game.h"

t_point	transform_point(float mat[3][3], t_point point)
{
	t_point	result;

	result.x = mat[0][0] * point.x + mat[0][1] * point.y + mat[0][2];
	result.y = mat[1][0] * point.x + mat[1][1] * point.y + mat[1][2];
	return (result);
}

void	get_matrix(t_minimap *minimap, float mat[3][3])
{
	mat[0][0] = minimap->ref_x * cosf(minimap->r);
	mat[0][1] = minimap->ref_y * -sinf(minimap->r);
	mat[0][2] = minimap->x;
	mat[1][0] = minimap->ref_x * sinf(minimap->r);
	mat[1][1] = minimap->ref_y * cosf(minimap->r);
	mat[1][2] = minimap->y;
	mat[2][0] = 0;
	mat[2][1] = 0;
	mat[2][2] = 1;
}
