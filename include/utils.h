/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 16:05:02 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/13 18:48:49 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

// Resolution and FOV
# define WIDTH 1080
# define HEIGHT 720
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

int			iclamp(int value, int min, int max);
float		fclamp(float value, float min, float max);
int			is_in_box(int x, int y, int w, int h);
int			is_in_circle(float x, float y, float radius);
t_vector	add_vector(t_vector v1, t_vector v2);
t_vector	sub_vector(t_vector v1, t_vector v2);
t_vector	multi_vector(t_vector v1, t_vector v2);
t_vector	div_vector(t_vector v1, t_vector v2);

#endif
