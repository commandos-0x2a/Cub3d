/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surrounded_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 12:01:57 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/15 12:02:29 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "map.h"
#include "libft.h"
#include <stdio.h>

t_stack	*init_stack(int x, int y)
{
	t_stack	*new;

	new = malloc(sizeof(*new));
	if (!new)
		return (NULL);
	new->x = x;
	new->y = y;
	new->next = NULL;
	return (new);
}

void	clear_stack(t_stack **stack)
{
	t_stack	*cur;
	t_stack	*tmp;

	cur = *stack;
	while (cur)
	{
		tmp = cur;
		cur = cur->next;
		free(tmp);
	}
	*stack = NULL;
}

t_stack	*add_to_stack(t_stack **stack, int x, int y)
{
	t_stack	*new;

	new = init_stack(x, y);
	if (!new)
		return (NULL);
	new->next = *stack;
	*stack = new;
	return (new);
}

t_stack	*add_player_position(t_grid *grid)
{
	size_t	y;
	size_t	x;

	y = 0;
	while (y < grid->h)
	{
		x = 0;
		while (x < grid->w)
		{
			if (ft_strchr("WESN", grid->raw[y * grid->w + x]))
				return (init_stack(x, y));
			x++;
		}
		y++;
	}
	return (NULL);
}

int	save_add_to_stack(t_stack **stack, t_grid *grid, int x, int y)
{
	char	c;

	c = grid->raw[y * grid->w + x];
	if (!is_in_box(x, y, grid->w, grid->h) || c == '@' || c == '1')
		return (0);
	if (add_to_stack(stack, x, y) == NULL)
		return (-1);
	return (1);
}
