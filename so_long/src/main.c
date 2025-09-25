/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 18:09:01 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/25 08:45:56 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include <stdio.h>

void		*mlx;
mlx_image_t	*frame;
t_so_long1	*so;


t_so_long1	*init_so_long1(const char *map_path, int width, int height);

void render(void *params)
{
	(void)params;
	ft_memcpy(frame->pixels, so->frame.tex.pixels, frame->width * frame->height * sizeof(int));	
}

/// @brief 
/// @param argc 
/// @param argv 
/// @return 
int	main(int argc, char **argv)
{
	const char	*filename;

	if (argc != 2)
	{
		return (1);
	}
	filename = argv[1];

	so = init_so_long1(filename, 1080, 720);
	if (!so)
	{
		perror("init_so_long");
		return (0);
	}
	if (!(mlx = mlx_init(1080, 720, "MLX42", true)))
	{
		puts(mlx_strerror(mlx_errno));
		return(1);
	}
	if ((frame = mlx_new_image(mlx, 1080, 720)) == NULL)
	{
		puts(mlx_strerror(mlx_errno));
		return (1);
	}
	if (mlx_image_to_window(mlx, frame, 0, 0) == -1)
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return(1);
	}

    mlx_loop_hook(mlx, rander_solong1, so);
    mlx_loop_hook(mlx, render, so);


    // mlx_loop_hook(mlx, key_release, so);
	// mlx_loop_hook(mlx, key_press, so);

	mlx_loop(mlx);
	// free map
	mlx_terminate(mlx);
	
	return (0);
}