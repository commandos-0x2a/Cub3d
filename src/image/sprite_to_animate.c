/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_to_animate.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 15:47:17 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/15 12:39:02 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sprite.h"
#include "animate.h"
#include <stdlib.h>
#include "libft.h"

void	sprite_next_frame(t_sprite_animate *anim)
{
	int		index;
	t_frame	*frame;

	index = anim->frame_index;
	if (anim->render_type == 2)
	{
		ft_memcpy(&anim->tex, &anim->group_used->frames[index],
			sizeof(mlx_texture_t));
		anim->frame_duration = anim->group_used->intervals[index];
	}
	else
	{
		frame = &anim->spr->frames[index];
		if (frame->type == 2)
		{
			if (frame->u.group.nb_frame > 0)
				ft_memcpy(&anim->tex, &frame->u.group.frames[0],
					sizeof(mlx_texture_t));
		}
		else
			ft_memcpy(&anim->tex, &frame->u.single, sizeof(mlx_texture_t));
	}
	index = (index + 1) % anim->frame_count;
	anim->frame_index = index;
}

t_group_frame	*get_group_used(t_sprite *spr, int group_idx)
{
	size_t				i;
	int					gi;

	i = 0;
	gi = 0;
	while (i < spr->nb_frame)
	{
		if (spr->frames[i].type == 2)
		{
			if (gi == group_idx)
				return (&spr->frames[i].u.group);
			gi++;
		}
		i++;
	}
	return (NULL);
}

t_sprite_animate	*sprite_to_animate(t_sprite *spr, int group_idx)
{
	t_sprite_animate	*anim;

	anim = ft_calloc(1, sizeof(t_sprite_animate));
	if (!anim)
		return (NULL);
	anim->spr = spr;
	anim->group_used = get_group_used(spr, group_idx);
	if (!anim->group_used)
	{
		anim->render_type = 1;
		anim->frame_duration = 0.2;
		anim->frame_count = spr->nb_frame;
	}
	else
	{
		anim->frame_count = anim->group_used->nb_frame;
		anim->frame_duration = anim->group_used->intervals[0];
		anim->render_type = 2;
	}
	anim->frame_index = 0;
	anim->last_animate = 0;
	return (anim);
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
		free_sprite(spr);
		return (NULL);
	}
	return (anim);
}
