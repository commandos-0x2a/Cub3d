#include "utils.h"
#include <fcntl.h>
#include <stdio.h>
#include "libft.h"
#include "MLX42/MLX42.h"

t_image_type	get_image_type(const char *path)
{
	char	*ext;

	ext = ft_strrchr(path, '.');
	if (!ext)
		return (IT_NONE);
	else if (ft_strcmp(ext, ".png") == 0)
		return (IT_PNG);
	else if (ft_strcmp(ext, ".xpm") == 0)
		return (IT_XPM);
	else if (ft_strcmp(ext, ".xpm") == 0)
		return (IT_SRP);
	return (IT_NONE);
}

t_texture   *open_image(const char *path)
{
	t_image_type	type;
	t_texture		*tex;

	type = get_image_type(path);
	if (type == IT_NONE)
		return (NULL);
	tex = ft_calloc(1, sizeof(*tex));
	if (!tex)
		return (NULL);
	tex->type = type;
	if (tex->type == IT_PNG)
		tex->ctx = mlx_load_png(path);
	else if (tex->type == IT_XPM)
		tex->ctx = mlx_load_xpm42(path);
	else if (tex->type == IT_SRP)
		tex->ctx = load_sprite_file(path);
}
