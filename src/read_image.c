#include "utils.h"
#include <fcntl.h>
#include <stdio.h>
#include "libft.h"

t_texture   *open_image(const char *path)
{
    int         fd;
    t_texture   *tex;

    fd = open(path, O_RDONLY);
    if (fd == -1)
    {
        perror(path);
        return (NULL);
    }
    // tex = 
}