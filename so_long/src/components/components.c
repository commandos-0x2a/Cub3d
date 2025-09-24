/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   components.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 18:26:52 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/24 17:43:29 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "components.h"
#include "game_schema.h"

static int	load_component(t_game_schema *gs, int i, char type, t_point loc)
{
	t_object	*component;

	component = NULL;
	if (type == 'E')
		component = (t_object *)init_boat(gs);
	else if (type == 'C')
		component = (t_object *)init_tree(gs, i);
	else if (type == 'F')
		component = (t_object *)init_fire(gs);
	else if (type == 'P')
		component = (t_object *)init_player(gs);
	if (!component)
		return (-1);
	component->relative_location = (t_point){(loc.x * 2 + 1) * 64, \
												(loc.y * 2 + 1) * 64};
	component->parent_location = (void *)&gs->camera.obj.absolute_location;
	add_children(&gs->components, component);
	return (0);
}

int	load_components(t_game_schema *gs, t_grid *o_map)
{
	uint32_t	r;
	uint32_t	c;
	uint32_t	i;

	i = 0;
	r = -1;
	while (++r < o_map->rows)
	{
		c = -1;
		while (++c < o_map->cols)
			if (ft_strchr("PECF", o_map->blocks[r][c]))
				if (load_component(gs, i++, o_map->blocks[r][c], \
									(t_point){c, r}) != 0)
					return (-1);
	}
	return (0);
}

static int	object_cmp(t_object *obj1, t_object *obj2)
{
	int	a;
	int	b;

	a = obj1->relative_location.y;
	b = obj2->relative_location.y;
	if (a < b)
		return (-1);
	if (a > b)
		return (1);
	return (0);
}

static void	swap_objects(t_object **a, t_object **b)
{
	t_object	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	sort_objects(t_object **objects, int nb_object)
{
	int	i;
	int	j;
	int	test;

	i = 0;
	while (i < nb_object - 1)
	{
		j = i + 1;
		test = -1;
		while (test < 0 && j < nb_object)
		{
			test = object_cmp(objects[i], objects[j]);
			j++;
		}
		if (test > 0)
			swap_objects(objects + i, objects + j - 1);
		i++;
	}
}
