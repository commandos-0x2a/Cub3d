/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 18:09:01 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/24 18:10:11 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void		*mlx;
mlx_image_t	*frame;
t_so_long1	*so;

int	main(int argc, char **argv)
{
	const char	*filename;

	if (argc != 2)
	{
		return (1);
	}
	filename = argv[1];

	if (!(mlx = mlx_init(500, 500, "MLX42", true)))
	{
		puts(mlx_strerror(mlx_errno));
		return(1);
	}
	so = init_so_long1("maps/map.ber", 500, 500);
	if (!so)
		return (0);
	
	if ((mlx_new_image(mlx, 500, 500)) == NULL)
		return (1);
	if (mlx_image_to_window(mlx, frame, 0, 0) == -1)
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return(1);
	}

    mlx_loop_hook(mlx, render_sprites, NULL);


    mlx_loop_hook(mlx, key_release, so);
	mlx_loop_hook(mlx, key_press, so);

	mlx_loop(mlx);
	// free map
	mlx_terminate(mlx);
	return (0);

	
	return (0);
}