/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   schema.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 14:38:11 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/25 10:04:32 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCHEMA_H
# define SCHEMA_H

# include "object.h"

typedef struct s_schema
{
	t_object	obj;
	void		*mlx_ptr;
	struct s_resources
	{
		t_image	*images;
		int		nb_images;
	}			resources;
	int			(*component_cmp)();
	int			(*check_schema)();
	int			(*load_schema)();
}	t_schema;

int			load_schema(void *_schema);
int			render_schema(void *_schema, mlx_texture_t *frame);
int			check_schema(void *_schema, void *data);
void		destroy_schema(void **_schema);
t_image		*schema_get_image_by_name(void *_schema, const char *name);

#endif