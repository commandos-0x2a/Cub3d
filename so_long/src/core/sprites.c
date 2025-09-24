/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 01:57:56 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/01/31 19:46:47 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sprites.h"

static void	run_animate(t_sprites *spr)
{
	void		(*animate)(t_sprites *);
	void		(*end_move)(t_sprites *);

	animate = spr->animate;
	if (animate)
		animate(spr);
	if (spr->delay > 0 && spr->run_animate)
	{
		end_move = spr->end_move;
		spr->timer++;
		if (spr->timer - spr->last_animate > spr->delay)
		{
			spr->last_animate = spr->timer;
			if (spr->max_index > 0)
			{
				spr->index = (spr->index + 1) % spr->max_index;
				if (spr->index == 0 && end_move)
					end_move(spr);
			}
		}
	}
}

void	animate_sprites(void *_spr)
{
	t_object	*spr;

	spr = (t_object *)_spr;
	if (spr->type[0] == 'S')
		run_animate(_spr);
	if (spr->childrens)
		animate_sprites(spr->childrens);
	if (spr->next)
		animate_sprites(spr->next);
}

int	render_sprites(void *_spr, t_image *frame, int layer)
{
	t_sprites	*spr;
	int			i;

	spr = (t_sprites *)_spr;
	i = 0;
	while (spr->clips && i < spr->nb_clips)
	{
		if (spr->clips[i].layer == layer)
			put_image_to_image(frame, spr->image, \
						spr->obj.draw_location, spr->clips[i]);
		i++;
	}
	return (0);
}

void	load_sprites(void *_spr)
{
	t_sprites	*spr;

	spr = (t_sprites *)_spr;
	load_object(spr);
	ft_strlcpy((char *)spr, "sprites", NAME_SIZE);
	((t_object *)spr)->type[0] = 'S';
	((t_object *)spr)->render = render_sprites;
	spr->clips = NULL;
	spr->nb_clips = 0;
	spr->delay = 0;
	spr->run_animate = 1;
	spr->last_animate = 0;
	spr->timer = 0;
	spr->index = 0;
	spr->max_index = 0;
	spr->animate = NULL;
	spr->end_move = NULL;
}
