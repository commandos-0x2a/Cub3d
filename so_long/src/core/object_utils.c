/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 08:01:22 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/01/27 08:01:51 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "object.h"

t_object	*get_children_by_name(void *_parent, const char *name)
{
	t_object	*parent;
	t_object	*current;

	parent = (t_object *)_parent;
	current = parent->childrens;
	while (current)
	{
		if (is_object_type(current, name))
			break ;
		current = current->next;
	}
	return (current);
}

t_object	*get_children_by_loacation(void *_parent, t_point location)
{
	t_object	*parent;
	t_object	*current;

	parent = (t_object *)_parent;
	current = parent->childrens;
	while (current)
	{
		if (ft_memcmp(&current->relative_location, \
						&location, sizeof(location)) == 0)
			break ;
		current = current->next;
	}
	return (current);
}

void	add_children(void *_parent, void *_child)
{
	t_object	*parent;
	t_object	*child;

	parent = (t_object *)_parent;
	child = (t_object *)_child;
	child->next = parent->childrens;
	parent->childrens = child;
}

int	is_object_type(void *obj, const char *type)
{
	return (ft_strncmp((char *)obj, type, NAME_SIZE) == 0);
}
