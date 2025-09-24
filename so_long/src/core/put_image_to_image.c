/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_image_to_image.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 23:22:42 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/24 17:42:21 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int	blend_color(uint32_t c1, uint32_t c2)
{
	unsigned char	*_c1;
	unsigned char	*_c2;
	unsigned char	c3[4];
	unsigned char	alpha;

	_c1 = (unsigned char *)&c1;
	_c2 = (unsigned char *)&c2;
	alpha = _c2[3];
	c3[0] = (_c1[0] * alpha + (255 - alpha) * _c2[0]) / 255;
	c3[1] = (_c1[1] * alpha + (255 - alpha) * _c2[1]) / 255;
	c3[2] = (_c1[2] * alpha + (255 - alpha) * _c2[2]) / 255;
	return (*(int *)c3);
}

void	put_image_to_image(t_image *dst, t_image *src, \
						t_point dst_loc, t_clip clip)
{
	uint32_t	x;
	uint32_t	y;
	uint32_t	*dc;
	uint32_t	sc;

	if (!dst || !src || clip.height < 0 || clip.width < 0)
		return ;
	y = -1;
	while (++y < clip.height && y + clip.y < src->tex->height \
								&& y + dst_loc.y < dst->tex->height)
	{
		if (y + clip.y < 0 || y + dst_loc.y < 0)
			continue ;
		x = -1;
		while (++x < clip.width && x + clip.x < src->tex->width \
								&& x + dst_loc.x < dst->tex->width)
		{
			if (x + clip.x < 0 || x + dst_loc.x < 0)
				continue ;
			dc = &((uint32_t *)dst->tex->pixels)[(y + dst_loc.y) * dst->tex->width + (x + dst_loc.x)];
			sc = ((uint32_t *)src->tex->pixels)[(y + clip.y) * src->tex->width + (x + clip.x)];
			*dc = blend_color(*dc, sc);
		}
	}
}
