/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:38:39 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/05 11:28:30 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "libft.h"

static void	ft_strjoin_gnl_help(char **new_line_p, char *buffer, \
									size_t i1, size_t i2)
{
	char	*new_line;
	size_t	len;
	char	*left;

	left = buffer + i2;
	len = i1 + i2;
	new_line = malloc(len + 1);
	if (!new_line)
	{
		if (*new_line_p)
			free(*new_line_p);
		*new_line_p = NULL;
		return ;
	}
	new_line[len] = '\0';
	while (i2--)
		new_line[--len] = buffer[i2];
	while (i1--)
		new_line[--len] = (*new_line_p)[i1];
	if (*new_line_p)
		free(*new_line_p);
	*new_line_p = new_line;
	ft_strlcpy(buffer, left, BUFFER_SIZE + 1);
}

static int	ft_strjoin_gnl(char **new_line, char *buffer)
{
	size_t	i1;
	size_t	i2;

	i1 = 0;
	i2 = 0;
	if (*new_line)
		i1 = ft_strlen(*new_line);
	while (buffer[i2] != '\n' && buffer[i2] != '\0')
		i2++;
	i2 += !!buffer[i2];
	ft_strjoin_gnl_help(new_line, buffer, i1, i2);
	if (!*new_line)
		return (0);
	i1 = 0;
	while ((*new_line)[i1] != '\0')
	{
		if ((*new_line)[i1] == '\n')
			return (1);
		i1++;
	}
	return (0);
}

static char	*error_handling_gnl(char **new_line, char *buffer)
{
	buffer[0] = '\0';
	if (*new_line)
	{
		free(*new_line);
		*new_line = NULL;
	}
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	buffer[FOPEN_MAX][BUFFER_SIZE + 1];
	char		*new_line;
	ssize_t		bytes_read;

	new_line = NULL;
	if (fd > FOPEN_MAX || fd < 0)
		return (NULL);
	if (!buffer[fd][0])
		bytes_read = read(fd, buffer[fd], BUFFER_SIZE);
	else
		bytes_read = ft_strlen(buffer[fd]);
	while (bytes_read)
	{
		if (bytes_read == -1)
			return (error_handling_gnl(&new_line, buffer[fd]));
		buffer[fd][bytes_read] = '\0';
		if (ft_strjoin_gnl(&new_line, buffer[fd]))
			return (new_line);
		if (!new_line)
			return (error_handling_gnl(&new_line, buffer[fd]));
		bytes_read = read(fd, buffer[fd], BUFFER_SIZE);
	}
	return (new_line);
}
