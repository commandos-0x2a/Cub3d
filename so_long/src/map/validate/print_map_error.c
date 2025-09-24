/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_map_error.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 11:43:13 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/24 17:46:24 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include "ft_printf.h"

void	print_grid(t_grid *grid)
{
	uint32_t	r;
	uint32_t	c;

	r = -1;
	while (++r < grid->rows)
	{
		c = -1;
		while (++c < grid->cols)
			ft_printf("%c", grid->blocks[r][c]);
		ft_printf("\n");
	}
}

static void	print_error_grid(t_grid *p_grid, t_grid *o_grid)
{
	uint32_t		r;
	uint32_t		c;
	char	*color;

	r = -1;
	while (++r < o_grid->rows)
	{
		c = -1;
		while (++c < o_grid->cols)
		{
			color = "\e[0;39m";
			if (ft_islower(p_grid->blocks[r][c]))
				color = "\e[0;32m";
			else if (ft_isupper(p_grid->blocks[r][c]))
				color = "\e[0;91m";
			if (p_grid->blocks[r][c] == 'F')
				color = "\e[1;35m";
			if (o_grid->blocks[r][c] == '0')
				ft_fprintf(2, "%c\e[0;39m", ' ');
			else
				ft_fprintf(2, "%s%c\e[0;39m", color, o_grid->blocks[r][c]);
		}
		ft_fprintf(2, "\n");
	}
}

int	print_error_line(char **lines, int line_no, int char_no, char *msg)
{
	int		i;
	int		n;
	size_t	msg_len;

	i = line_no - 2;
	if (i < 0)
		i = 0;
	while (lines[i] && i < line_no)
	{
		fprintf(stderr, "%i: %n%s\n", i + 1, &n, lines[i]);
		i++;
	}
	fprintf(stderr, "\e[0;91m%i: %n%s\n", i + 1, &n, lines[i]);
	n -= 7;
	fprintf(stderr, "%*s^\n", n + char_no, "");
	msg_len = ft_strlen(msg);
	n = n + char_no - msg_len / 2;
	if (n < 0)
		n = 0;
	fprintf(stderr, "%*s%s\n\n\e[0;39m", n, "", msg);
	return (-1);
}

int	print_error_number1(t_grid *grid, int *nb)
{
	ft_fprintf(2, "\n");
	print_grid(grid);
	ft_fprintf(2, "\n");
	ft_fprintf(2, "\e[0;91m");
	if (nb[0] == 0)
		ft_fprintf(2, "Not found any player (P) in map\n");
	if (nb[0] > 1)
		ft_fprintf(2, "Find many players (P) in map\n");
	if (nb[1] == 0)
		ft_fprintf(2, "Not found any exit (E) in map\n");
	if (nb[1] > 1)
		ft_fprintf(2, "Find many exits (E) in map\n");
	if (nb[2] == 0)
		ft_fprintf(2, "Not found any collectible (C) in map\n");
	ft_fprintf(2, "\e[0;39m");
	return (-1);
}

int	print_error_number2(t_grid *p_grid, t_grid *o_grid, int *nb)
{
	ft_fprintf(2, "\n");
	print_error_grid(p_grid, o_grid);
	ft_fprintf(2, "\n");
	ft_fprintf(2, "\e[0;91m");
	if (nb[1] != 0)
		ft_fprintf(2, "Player cannot access to exit\n");
	if (nb[2] != 0)
		ft_fprintf(2, "Player cannot access to '%d' collectibles\n", nb[2]);
	ft_fprintf(2, "\n");
	ft_fprintf(2, "\e[0;39m");
	free_grid(p_grid);
	return (-1);
}
