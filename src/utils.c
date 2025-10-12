/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 09:54:04 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/12 16:03:30 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

int	iclamp(int value, int min, int max)
{
	return (int)fmax(min, fmin(value, max));
}

float	fclamp(float value, float min, float max)
{
	return fmaxf(min, fminf(value, max));
}

int	is_in_box(int x, int y, int w, int h)
{
	return (x >= 0 && x < w && y >= 0 && y < h);
}

int	is_in_circle(float x, float y, float radius)
{
	return (powf(x, 2) + powf(y, 2) <= radius * 2);
}
