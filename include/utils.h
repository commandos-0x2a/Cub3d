/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 16:05:02 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/14 21:24:09 by yaltayeh         ###   ########.fr       */
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

// NUM_RAYS: the amount of rays that will be shot from the player. More gives more detailed and less prone to hidden walls or entites
// due to the lack of rays.
# define NUM_RAYS WIDTH

typedef struct	s_vector
{
	float	x;
	float	y;
}	t_vector;

#endif
