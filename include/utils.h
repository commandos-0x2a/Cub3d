/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 16:05:02 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/16 09:00:34 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

// Resolution
# define WIDTH 1080
# define HEIGHT 720

// PIXEL SCALE: is for scaling the entites movement. because moving 1px to right in a 1080 720 res is slow as hell
# define PIXEL_SCALE 1

// ( int ) FOV: determines the range the player will see from.
// higher number results in more angle, less number will result in less angle to see through
// Giving the "zoomed" effect
# define FOV 60

# define TILE_SIZE 1

typedef struct	s_vector
{
	float	x;
	float	y;
}	t_vector;

typedef enum e_image_type
{
	IT_NONE,
	IT_PNG,
	IT_XPM,
	IT_SRP,
}	t_image_type;

typedef struct s_texture
{
	int		type;
	void	*ctx;
}	t_texture;

#endif
