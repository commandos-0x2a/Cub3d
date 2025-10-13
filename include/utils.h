/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 16:05:02 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/13 11:05:13 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

// Resolution
# define WIDTH 1080
# define HEIGHT 720

// ( int ) FOV: determines the range the player will see from.
// higher number results in more angle, less number will result in less angle to see through
// Giving the "zoomed" effect
# define FOV 60

# define TILE_SIZE 1

typedef struct s_vector
{
	float	x;
	float	y;
}	t_vector;

typedef struct s_vector	t_point;

typedef enum e_image_type
{
	IT_NONE,
	IT_PNG,
	IT_XPM,
	IT_SRP,
}	t_image_type;

int		iclamp(int value, int min, int max);
float	fclamp(float value, float min, float max);
int		is_in_box(int x, int y, int w, int h);
int		is_in_circle(float x, float y, float radius);

t_vector	add_vector(t_vector v1, t_vector v2);
t_vector	sub_vector(t_vector v1, t_vector v2);
t_vector	multi_vector(t_vector v1, t_vector v2);
t_vector	div_vector(t_vector v1, t_vector v2);

#endif
