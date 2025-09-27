/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_to_animate.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 15:47:17 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/27 15:04:15 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sprite.h"
#include "animate.h"
#include <stdlib.h>
#include "libft.h"

static void sprite_next_frame(t_sprite_animate *anim)
{
	int index;
	t_frame *frame;

	index = anim->anim.frame_index;
	if (anim->render_type == 2)
	{
		ft_memcpy(&anim->anim.tex, &anim->group_used->frames[index], sizeof(mlx_texture_t));
		anim->anim.frame_duration = anim->group_used->intervals[index];
	}
	else
	{
		frame = &anim->spr->frames[index];
		if (frame->type == 2)
		{
			if (frame->u.group.nb_frame > 0)
				ft_memcpy(&anim->anim.tex, &frame->u.group.frames[0], sizeof(mlx_texture_t));
		}
		else
			ft_memcpy(&anim->anim.tex, &frame->u.single, sizeof(mlx_texture_t));
	}
	index = (index + 1) % anim->anim.frame_count;
	anim->anim.frame_index = index;
	// printf("%s: %p      %d\n", __func__, anim->anim.tex.pixels, anim->anim.frame_count);
}

t_sprite_animate *sprite_to_animate(t_sprite *spr, int group_idx)
{
	t_sprite_animate	*sanim;
	size_t 				i;
	int 				gi;

	sanim = calloc(1, sizeof(t_sprite_animate));
	if (!sanim)
		return (NULL);
	sanim->spr = spr;
	i = 0;
	gi = 0;
	while (i < spr->nb_frame)
	{
		if (spr->frames[i].type == 2)
		{
			if (gi == group_idx)
			{
				sanim->group_used = &spr->frames[i].u.group;
				break;
			}
			gi++;
		}
		i++;
	}

	if (!sanim->group_used)
	{
		sanim->render_type = 1;
		sanim->anim.frame_duration = 0.2;
		sanim->anim.frame_count = spr->nb_frame;
	}
	else
	{
		sanim->anim.frame_count = sanim->group_used->nb_frame;
		sanim->anim.frame_duration = sanim->group_used->intervals[0];
		sanim->render_type = 2;
	}

	sanim->anim.frame_index = 0;
	sanim->anim.last_animate = 0;
	sanim->anim.next_frame = sprite_next_frame;
	sanim->anim.ctx = spr;
	return (sanim);
}

t_sprite_animate	*load_animate_sprite(const char *filename, int group_idx)
{
	t_sprite			*spr;
	t_sprite_animate	*anim;

	spr = load_sprite(filename);
	if (!spr)
		return (NULL);
	anim = sprite_to_animate(spr, group_idx);
	if (!anim)
	{
		// free_sprite(spr);
		return (NULL);
	}
	return (anim);
}
