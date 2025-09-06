#include "libft.h"

#define BUFFER_SIZE 1000

char	move_content(char **dst_r, const char *src, char *dstend)
{
	char	*dst;

	dst = *dst_r;
	while (*src && *src != '\n' && dst < dstend)
		*dst++ = *src++;
	if (dst >= dstend)
		return (-1);
	*dst = *src;
	return (0);
}

int	get_next_line(int fd, char *dst, size_t dstsize)
{
	static char	left[BUFFER_SIZE];
	char		buf[BUFFER_SIZE + 1];
	char		*dstend;
	ssize_t		ret;

	dstend = dst + dstsize;
	ret = move_content(&dst, left, dstend);
	left[0] = '\0';
	if (ret == -1)
		return (-1);
	
	while (ret != 0 && *dst != '\n')
	{
		ret = read(fd, buf, BUFFER_SIZE);
		if (ret <= 0)
			return (ret);
		buf[ret] = '\0';
		cur = buf;
		while (*cur && *cur != '\n' && len < dstsize)
			dst[len++] = *cur++;
	}
	if (len >= dstsize)
		return (-1);
	return (0);
}
