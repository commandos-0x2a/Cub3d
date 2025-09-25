/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   components.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 18:27:06 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/02/02 11:52:09 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMPONENTS_H
# define COMPONENTS_H

# include "sprites.h"
# include "player.h"
# include "game_schema.h"

typedef struct s_grid			t_grid;
typedef struct s_game_schema	t_game_schema;

typedef struct s_fire
{
	t_sprites	spr;
	t_clip		clips[2];
}	t_fire;

typedef struct t_tree
{
	t_sprites		spr;
	int				status;
	t_clip			clip;
	int				health;
}	t_tree;

enum e_boat_directions
{
	HORIZONTAL,
	DIAGONAL,
	VERTICAL,
};

typedef struct s_boat
{
	t_sprites		spr;
	t_clip			clip;
	int				direction;
	int				move_lock;
	int				is_move;
	void			*game;
	t_game_schema	*gs;
}	t_boat;

t_tree		*init_tree(t_game_schema *gs, int i);
t_boat		*init_boat(t_game_schema *gs);
t_fire		*init_fire(t_game_schema *gs);
int			load_components(t_game_schema *gs, t_grid *o_map);
void		sort_objects(t_object **objects, int nb_object);
int			damage_tree(t_tree *tree, int damage);

#endif