/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 15:58:06 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/01/31 23:05:21 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "object.h"

void	load_object(void *obj)
{
	ft_bzero(obj, sizeof(t_object));
	ft_strlcpy((char *)obj, "object", NAME_SIZE);
}

void	render_object(void *_obj, t_image *frame, int layer)
{
	t_object	*obj;
	int			(*render)(void *, t_image *, int);

	obj = (t_object *)_obj;
	if (obj->next)
		render_object(obj->next, frame, layer);
	render = obj->render;
	if (obj->childrens)
		render_object(obj->childrens, frame, layer);
	if (render)
		render(obj, frame, layer);
}

void	update_object(void *_obj)
{
	t_object	*obj;
	void		(*update)(void *);	

	obj = (t_object *)_obj;
	update = obj->update;
	if (update)
		update(obj);
	obj->absolute_location = obj->relative_location;
	if (obj->parent_location)
	{
		obj->absolute_location.x += obj->parent_location->x;
		obj->absolute_location.y += obj->parent_location->y;
	}
	obj->draw_location = obj->absolute_location;
	obj->draw_location.x -= obj->center_point.x;
	obj->draw_location.y -= obj->center_point.y;
	if (obj->childrens)
		update_object(obj->childrens);
	if (obj->next)
		update_object(obj->next);
}

void	defult_destroy_object(void **_obj)
{
	free(*_obj);
	*_obj = NULL;
}

/*
ft_fprintf(2, "destroy %s (%d, %d)\n", 
			(char *)obj, 
			obj->relative_location.x, 
			obj->relative_location.y);
*/

void	destroy_object(void **_obj)
{
	t_object	*obj;
	void		(*destroy)(void **);

	obj = (t_object *)*_obj;
	destroy = obj->destroy;
	if (obj->childrens)
		destroy_object(&obj->childrens);
	if (obj->next)
		destroy_object(&obj->next);
	if (destroy)
		destroy(_obj);
}
