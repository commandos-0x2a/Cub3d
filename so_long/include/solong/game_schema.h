/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_schema.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/09 23:34:56 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/25 08:06:40 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_SCHEMA_H
# define GAME_SCHEMA_H

# include "object.h"
# include "schema.h"
# include "map.h"
# include "camera.h"
# include "components.h"

enum e_banner_theme
{
	B_RED,
	B_DARK_RED,
	B_NEUTRAL,
	B_DARK_NEUTRAL,
	B_WHITE,
	B_DARK_WHITE,
};

typedef struct s_banner
{
	t_object	obj;
	t_image		*image;
	t_point		offset;
	int			*logs_collected;
	int			*movement;
}	t_banner;

typedef struct s_image_info
{
	char	*path;
	char	*name;
}	t_image_info;

typedef struct s_game_schema
{
	t_schema	schema;
	t_object	components;
	t_map		map;
	t_camera	camera;
	t_banner	banner;
}	t_game_schema;

t_game_schema	*init_game_schema(void);
void			load_banner(t_banner *banner, t_game_schema *gs, int theme);

int				open_xpm_file(t_image *image, \
						char *filename, const char *img_name);
int	open_png_file(t_image *image,\
						char *filename, const char *img_name);
t_object		*pop_player(t_object *components);
void			insert_player(t_object *current, t_object *prev, \
						t_object *player, t_object *components);

#endif