/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   schema.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 15:51:32 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/24 17:43:08 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "schema.h"

int	load_schema(void *_schema)
{
	int			(*load_schema_ptr)(t_schema *);
	t_schema	*schema;

	schema = (t_schema *)_schema;
	if (!schema)
		return (-1);
	if (schema->load_schema)
	{
		load_schema_ptr = schema->load_schema;
		if (load_schema_ptr)
			return (load_schema_ptr(schema));
		return (-1);
	}
	return (0);
}

int	check_schema(void *_schema, void *data)
{
	int			(*check_schema_ptr)(void *, void *);
	t_schema	*schema;

	schema = (t_schema *)_schema;
	if (!schema)
		return (-1);
	check_schema_ptr = schema->check_schema;
	if (check_schema_ptr)
		return (check_schema_ptr(schema, data));
	return (0);
}

void	destroy_schema(void **_schema)
{
	t_schema	*schema;
	int			i;

	schema = (t_schema *)*_schema;
	if (schema->resources.images)
	{
		i = -1;
		while (++i < schema->resources.nb_images)
				free(schema->resources.images[i].tex->pixels);
		free(schema->resources.images);
	}
	schema->resources.images = NULL;
}

t_image	*schema_get_image_by_name(void *_schema, const char *name)
{
	int			i;
	t_schema	*schema;

	schema = (t_schema *)_schema;
	i = 0;
	while (i < schema->resources.nb_images)
	{
		if (ft_strncmp((char *)&schema->resources.images[i], \
							name, NAME_SIZE) == 0)
			return (&schema->resources.images[i]);
		i++;
	}
	return (NULL);
}
