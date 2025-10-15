/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_color.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/13 19:06:34 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/15 13:10:38 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <stdio.h>

static int	get_0_255(int *color, char **line_r, int offset, int check_comma)
{
	int		slice;
	char	*line;

	line = *line_r;
	slice = ft_atoi_r(&line);
	if (*line_r == line)
		return (-1);
	while (*line && ft_isspace(*line))
		line++;
	*line_r = line;
	if (slice < 0 || slice > 255)
		return (-1);
	if ((check_comma && *line != ',') || (!check_comma && *line != '\0'))
		return (-1);
	if (check_comma)
		(*line_r)++;
	*color |= slice << offset;
	return (0);
}

int	copy_color(int *color, char *line)
{
	int	err;

	*color = 0xff;
	err = get_0_255(color, &line, 24, 1);
	if (err != 0)
		return (err);
	err = get_0_255(color, &line, 16, 1);
	if (err != 0)
		return (err);
	err = get_0_255(color, &line, 8, 0);
	return (err);
}
