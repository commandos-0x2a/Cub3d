/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 09:54:04 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/07 17:38:42 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

int	max(int a, int b)
{
	if (a >= b)
		return (a);
	return (b);
}

int	iclamp(int value, int min, int max)
{
	return (int)fmax(min, fmin(value, max));
}

float	fclamp(float value, float min, float max)
{
	return fmaxf(min, fminf(value, max));
}

double	clamp(double value, double min, double max)
{
	return fmax(min, fmin(value, max));
}
