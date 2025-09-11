#include "map.h"
#include "libft.h"
#include <stdio.h>

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

t_stack	*full_all_empty_space(t_grid *grid)
{
	size_t	y;
	size_t	x;
	t_stack	*stack;

	y = -1;
	stack = NULL;
	while (y <= grid->h)
	{
		x = -1;
		while (x <= grid->w)
		{
			if (x < 0 || y < 0 || x >= grid->w || y >= grid->h
				|| grid->raw[y * grid->w + x] == ' ')
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
	if (x < 0 || y < 0 || x >= (int)grid->w || y >= (int)grid->h 
		|| c == '@' || c == '1')
		return (0);
	// printf("%s(%d, %d) = %c\n", __func__, x, y, grid->raw[y * grid->w + x]);
	if (add_to_stack(stack, x, y) == NULL)
		return (-1);
	return (1);
}

int	add_block_surrounded(t_stack **stack, t_grid *grid, int x, int y)
{
	if (save_add_to_stack(stack, grid, x, y - 1) < 0
		|| save_add_to_stack(stack, grid, x + 1, y) < 0
		|| save_add_to_stack(stack, grid, x - 1, y) < 0
		|| save_add_to_stack(stack, grid, x, y + 1) < 0)
		return (0);
	return (1);
}

t_grid	*copy_grid(t_grid *grid)
{
	t_grid	*new_grid;

	new_grid = malloc(sizeof(*new_grid));
	if (!new_grid)
		return (NULL);
	new_grid->w = grid->w;
	new_grid->h = grid->h;
	new_grid->raw = malloc(grid->w * grid->h);
	ft_memcpy(new_grid->raw, grid->raw, grid->w * grid->h);
	return (new_grid);
}

void	print_stack(t_stack *stack, t_grid *grid)
{
	char	c;

	while (stack)
	{
		c = grid->raw[stack->y * grid->w + stack->x];
		printf("(%d, %d) = %c\n", stack->x, stack->y, c);
		stack = stack->next;
	}
	
}

int	valid_surrounded_wall(t_map *map)
{
	t_stack *cur;
	t_stack	*next;
	t_grid	*tmp_grid;
	char	*c;

	tmp_grid = copy_grid(&map->grid);
	if (!tmp_grid)
		return (0);
	cur = add_player_position(tmp_grid);
	if (!cur)
	{
		free(tmp_grid->raw);
		free(tmp_grid);
		return (0);
	}
	while (cur)
	{
		next = cur->next;
		c = &tmp_grid->raw[cur->y * tmp_grid->w + cur->x];
		if (*c == ' '
			|| cur->y < 0 || cur->y >= (int)tmp_grid->h
			|| cur->x < 0 || cur->x >= (int)tmp_grid->w)
		{
			free(tmp_grid->raw);
			free(tmp_grid);
			clear_stack(&cur);
			return (0);
		}
		if (*c != '1')
		{
			add_block_surrounded(&next, tmp_grid, cur->x, cur->y);
			*c = '@';
		}
		free(cur);
		cur = next;
		// print_stack(cur, tmp_grid);
		// print_grid(tmp_grid);
		// char c;
		// read(0, &c, 1);
		// if (c == 'q')
		// 	break;
		// printf("\n\n\n\n\n");
	}
	print_grid(tmp_grid);
	free(tmp_grid->raw);
	free(tmp_grid);
	clear_stack(&cur);
	return (1);
}
