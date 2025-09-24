/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:00:00 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/24 13:58:18 by yaltayeh         ###   ########.fr       */
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

void       	*mlx;
mlx_image_t	*frame;
t_sprite	*spr;
size_t		frame_index;

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
	if (mlx_get_time() - last > spr->frames[0].u.group.intervals[0])
		frame_index = (frame_index + 1) % (spr->frames[0].u.group.nb_frame);
	else
		return ;

	
	last = mlx_get_time();
	
	mlx_texture_t	*tex;
	
	// tex = &spr->frames[frame_index].u.single;

	tex = &spr->frames[0].u.group.frames[frame_index];

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
	printf("Testing sprite reader with file: %s\n", filename);
	printf("===========================================\n");
	spr = load_sprite(filename);
	if (!spr)
	{
		printf("read spr error\n");
		return (1);
	}
	printf("\n=== TEST COMPLETE ===\n");

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