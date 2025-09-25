/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_image_to_image.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/30 23:22:42 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/25 10:05:53 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

uint32_t	blend_color(uint32_t dst, uint32_t src)
{
	uint8_t a_src = (src >> 24) & 0xFF;
    uint8_t r_src = (src >> 16) & 0xFF;
    uint8_t g_src = (src >> 8)  & 0xFF;
    uint8_t b_src = (src)       & 0xFF;

    uint8_t a_dst = (dst >> 24) & 0xFF;
    uint8_t r_dst = (dst >> 16) & 0xFF;
    uint8_t g_dst = (dst >> 8)  & 0xFF;
    uint8_t b_dst = (dst)       & 0xFF;

    // Normalize alpha to [0,1] but using integer math (0–255)
    int a_out = a_src + ((255 - a_src) * a_dst + 127) / 255;

    if (a_out == 0) return 0; // fully transparent

    int r_out = (r_src * a_src + r_dst * a_dst * (255 - a_src) / 255) / a_out;
    int g_out = (g_src * a_src + g_dst * a_dst * (255 - a_src) / 255) / a_out;
    int b_out = (b_src * a_src + b_dst * a_dst * (255 - a_src) / 255) / a_out;

    return ((a_out & 0xFF) << 24) |
           ((r_out & 0xFF) << 16) |
           ((g_out & 0xFF) << 8)  |
           (b_out & 0xFF);
}

void	put_image_to_image(mlx_texture_t *dst, mlx_texture_t *src, \
						t_point dst_loc, t_clip clip)
{
	uint32_t	x;
	uint32_t	y;
	uint32_t	*dc;
	uint32_t	sc;

	if (!dst || !src || clip.height < 0 || clip.width < 0)
		return ;
	y = -1;
	while (++y < clip.height && y + clip.y < src->height \
								&& y + dst_loc.y < dst->height)
	{
		if (y + clip.y < 0 || y + dst_loc.y < 0)
			continue ;
		x = -1;
		while (++x < clip.width && x + clip.x < src->width \
								&& x + dst_loc.x < dst->width)
		{
			if (x + clip.x < 0 || x + dst_loc.x < 0)
				continue ;
			dc = &((uint32_t *)dst->pixels)[(y + dst_loc.y) * dst->width + (x + dst_loc.x)];
			sc = ((uint32_t *)src->pixels)[(y + clip.y) * src->width + (x + clip.x)];
			*dc = blend_color(*dc, sc);
		}
	}
}
