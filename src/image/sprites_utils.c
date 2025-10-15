/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 12:33:25 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/15 12:34:13 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sprite.h"
#include <unistd.h>

int	read_exact(int fd, void *buffer, size_t size)
{
	ssize_t	bytes_read;
	ssize_t	total_read;
	char	*buf_ptr;

	if (!buffer)
		return (SPRITE_ERROR_NULL_PTR);
	buf_ptr = (char *)buffer;
	total_read = 0;
	while (total_read < (ssize_t)size)
	{
		bytes_read = read(fd, buf_ptr + total_read, size - total_read);
		if (bytes_read <= 0)
			return (SPRITE_ERROR_HEADER);
		total_read += bytes_read;
	}
	return (SPRITE_SUCCESS);
}
