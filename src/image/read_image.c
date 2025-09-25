#include "utils.h"
#include <stdio.h>
#include "libft.h"
#include "game.h"
#include "MLX42/MLX42.h"
#include "sprite.h"

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

mlx_texture_t   *open_image(t_game *game, const char *path)
{
	t_image_type	type;
	mlx_texture_t	*tex;

	type = get_image_type(path);

	if (type == IT_PNG)
		tex = mlx_load_png(path);
	else if (type == IT_XPM)
		tex = mlx_load_xpm42(path);
	else if (type == IT_SRP)
	{
		tex = load_sprite(path);
		if (tex)
		{
			
		}
	}
	else
		tex = NULL;
	return (tex);	
}
