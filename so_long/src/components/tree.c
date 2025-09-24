/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/06 20:24:14 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/01/30 10:02:00 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "components.h"

void	animate_tree(t_tree *tree)
{
	tree->clip = (t_clip){56 * tree->status, 0, 56, 94, 1};
}

int	damage_tree(t_tree *tree, int damage)
{
	if (tree->status != 2)
	{
		tree->health -= damage;
		if (tree->health <= 0)
		{
			tree->status = 2;
			return (1);
		}
	}
	return (0);
}

t_tree	*init_tree(t_game_schema *gs, int i)
{
	t_tree			*tree;

	tree = malloc(sizeof(t_tree));
	if (!tree)
		return (NULL);
	load_sprites((void *)tree);
	ft_strlcpy((char *)tree, "tree", NAME_SIZE);
	((t_object *)tree)->center_point = (t_point){56 / 2, 84};
	((t_object *)tree)->destroy = defult_destroy_object;
	((t_sprites *)tree)->image = schema_get_image_by_name(gs, "tree");
	((t_sprites *)tree)->clips = &tree->clip;
	((t_sprites *)tree)->nb_clips = 1;
	((t_sprites *)tree)->delay = 0;
	((t_sprites *)tree)->animate = animate_tree;
	tree->clip = (t_clip){0, 0, 56, 94, 1};
	tree->status = i & 1;
	tree->health = 120 - 20 * (i & 1);
	return (tree);
}
