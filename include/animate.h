/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animate.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 18:22:09 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/10/13 13:27:05 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANIMATE_H
# define ANIMATE_H

# include <MLX42/MLX42.h>
# include "sprite.h"

typedef struct s_sprite_animate
{
	mlx_texture_t	tex;
	int				frame_count;
	int				frame_index;
	double			frame_duration;
	double			last_animate;
	t_sprite		*spr;
	int				render_type;
	t_group_frame	*group_used; // using when render_type 2
}	t_sprite_animate;

t_sprite_animate	*sprite_to_animate(t_sprite *spr, int group_idx);
t_sprite_animate	*load_animate_sprite(const char *filename, int group_idx);
void				sprite_next_frame(t_sprite_animate *anim);

#endif
