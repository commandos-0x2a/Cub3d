/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 12:05:54 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/01/31 19:47:13 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECT_H
# define OBJECT_H

# include "utils.h"

typedef struct s_object
{
	char		name[NAME_SIZE];
	char		type[NAME_SIZE];
	t_point		draw_location;
	t_point		absolute_location;
	t_point		relative_location;
	t_point		center_point;
	t_point		*parent_location;
	void		*next;
	void		*childrens;
	void		(*update)();
	int			(*render)();
	void		(*destroy)();
}	t_object;

int			is_object_type(void *obj, const char *type);
void		add_children(void *_parent, void *_child);
void		load_object(void *_obj);
void		render_object(void *obj, t_image *frame, int layer);
void		update_object(void *_obj);
void		destroy_object(void **_obj);
void		defult_destroy_object(void **_obj);
int			collision_object(void *_obj, void *tangible);
t_object	*get_children_by_name(void *_parent, const char *name);
t_object	*get_children_by_loacation(void *_parent, t_point location);

#endif