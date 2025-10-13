/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hassende <hassende@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 09:54:04 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/13 17:46:38 by hassende         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

int	iclamp(int value, int min, int max)
{
	return ((int)fmax(min, fmin(value, max)));
}

float	fclamp(float value, float min, float max)
{
	return (fmaxf(min, fminf(value, max)));
}

int	is_in_box(int x, int y, int w, int h)
{
	return (x >= 0 && x < w && y >= 0 && y < h);
}

int	is_in_circle(float x, float y, float radius)
{
	return (powf(x, 2) + powf(y, 2) <= radius * 2);
}
