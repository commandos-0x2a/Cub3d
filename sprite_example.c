/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_example.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 22:35:00 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/16 09:34:59 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "../include/sprite.h"

/**
 * @brief Example of batch loading multiple sprites
 */
int load_multiple_sprites(void)
{
	const char *sprite_files[] = {
		"bonfire.spr",
		"directions_64x64.spr",
		"test_additive.spr",
		NULL
	};
	int i;
	int loaded_count = 0;
	
	printf("Loading multiple sprites...\n");
	
	for (i = 0; sprite_files[i] != NULL; i++)
	{
		printf("\n--- Loading sprite %d ---\n", i + 1);
		if (load_game_sprite(sprite_files[i]) == 0)
			loaded_count++;
		else
			printf("Failed to load: %s\n", sprite_files[i]);
	}
	
	printf("\nLoaded %d out of %d sprites successfully.\n", loaded_count, i);
	return (loaded_count);
}

int main(void)
{
	printf("Sprite Loading Example\n");
	printf("======================\n");
	
	// Example 1: Load a single sprite
	printf("\n=== Single Sprite Loading ===\n");
	if (load_game_sprite("bonfire.spr") != 0)
		printf("Single sprite loading failed\n");
	
	// Example 2: Load multiple sprites
	printf("\n=== Multiple Sprite Loading ===\n");
	load_multiple_sprites();
	
	return (0);
}