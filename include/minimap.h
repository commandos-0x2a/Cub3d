/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 03:13:58 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/12 07:14:55 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIMAP_H
# define MINIMAP_H

# define DRAWING_SCALE 50

# include "utils.h"

typedef struct	s_minimap
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

#endif
