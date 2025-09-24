/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:00:00 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/24 16:52:41 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// #include "../include/sprite_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include <stdio.h>
#include <MLX42/MLX42.h>
#include "game.h"
#include "libft.h"
#include "sprite.h"
#include "animate.h"

void       	*mlx;
mlx_image_t	*frame;
t_animate *anim;
double last;

void    render_sprites(void *param)
{
	// printf("%f\n", spr->frames[0].u.group.intervals[0]);
	(void)param;
	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
	{
		mlx_close_window(mlx);
		exit(0);
	}
	if (mlx_get_time() - last > anim->frame_duration)
		anim->frame_index = (anim->frame_index + 1) % (anim->frame_count);
	else
		return ;

	anim->next_frame(anim, anim->ctx);
	last = mlx_get_time();
	
	mlx_texture_t	*tex;
	
	// tex = &spr->frames[frame_index].u.single;

	tex = &anim->tex;

	for (int y = 0; y < tex->height; y++)
	{
		for (int x = 0; x < tex->width; x++)
		{
			uint32_t	c = ((uint32_t *)tex->pixels)[y * tex->width + x];
			mlx_put_pixel(frame, x, y, c);
		}
	}

}

int	main(int argc, char **argv)
{
	const char	*filename;

	if (argc != 2)
	{
		return (1);
	}
	filename = argv[1];
	anim = (void *)load_animate_sprite(filename, 0);
	if (!anim)
	{
		printf("read spr error\n");
		return (1);
	}
	printf("test\n");
	if (!(mlx = mlx_init(WIDTH, HEIGHT, "MLX42", true)))
	{
		puts(mlx_strerror(mlx_errno));
		return(1);
	}
	last = mlx_get_time();
	if (!(frame = mlx_new_image(mlx, WIDTH, HEIGHT)))
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return(1);
	}
	if (mlx_image_to_window(mlx, frame, 0, 0) == -1)
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return(1);
	}

    mlx_loop_hook(mlx, render_sprites, NULL);

	

	mlx_loop(mlx);
	// free map
	mlx_terminate(mlx);
	return (0);

	
	return (0);
}