#include "map.h"
#include "libft.h"

t_stack *init_stack(int x, int y)
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

t_stack *add_to_stack(t_stack **stack, int x, int y)
{
	t_stack *new;

	new = init_stack(x, y);
	if (!new)
		return (NULL);
	new->next = *stack;
	*stack = new;
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

t_stack	*full_all_space(t_map *map)
{
	size_t	y;
	size_t	x;
	t_stack	*stack;

	y = 0;
	stack = NULL;
	while (y < map->h)
	{
		x = 0;
		while (x < map->w)
		{
			if (map->grid[y * map->w + x] == ' ')
			{
				if (add_to_stack(&stack, x, y) == NULL)
				{
					clear_stack(&stack);
					return (NULL);
				}
			}
			x++;
		}
		y++;
	}
	return (stack);
}

t_stack	*add_block_surrounded(t_map *map, int x, int y)
{
	
}

int	valid_surrounded_wall(t_map *map)
{
	t_stack *cur;
	

	cur = full_all_space(map);
	if (!cur)
		return (0);
	while (cur)
	{
		
	}
	return (1);
}
