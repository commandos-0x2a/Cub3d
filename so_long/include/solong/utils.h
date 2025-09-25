/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/04 07:00:09 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/25 08:01:17 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stdlib.h>
# include <libft.h>
# include "ft_printf.h"
# include <MLX42/MLX42.h>
# include <stdio.h>

# define NAME_SIZE 16

typedef unsigned long		t_clock;

typedef struct s_image
{
	char			name[NAME_SIZE];
	mlx_texture_t	tex;
}	t_image;

typedef struct s_point
{
	uint32_t	x;
	uint32_t	y;
}	t_point;

typedef struct s_cell
{
	uint32_t	r;
	uint32_t	c;
}	t_cell;

typedef struct s_clip
{
	uint32_t	x;
	uint32_t	y;
	uint32_t	width;
	uint32_t	height;
	int	layer;
}	t_clip;

t_point	point_to_center(t_point center, t_point size);

void	put_image_to_image(t_image *dst, t_image *src, \
						t_point dst_loc, t_clip clip);

t_point	add_point(t_point p1, t_point p2);

int		is_new_movement(t_point old_location, t_point new_location);

#endif