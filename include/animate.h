/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animate.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 18:22:09 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/23 18:36:29 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANIMATE_H
# define ANIMATE_H

# include <MLX42/MLX42.h>

typedef struct s_animate
{
    mlx_texture_t   tex;
    int             frame_count;
    int             frame_index;
    double          frame_duration;
    double          elapsed_time;
    void            (*next_frame)(void *);
}   t_animate;


#endif
