/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_example.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 22:35:00 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/14 23:07:14 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "../include/sprite.h"

/**
 * @brief Example function showing how to load and use a sprite in your game
 * @param sprite_path Path to the sprite file
 * @return 0 on success, -1 on error
 */
int load_game_sprite(const char *sprite_path)
{
	t_sprite				*sprite;
	t_sprite_frame_header	*frame_header;
	unsigned char			*palette;
	unsigned char			*pixel_data;
	int						result;
	int						i, j;
	
	// Allocate structures
	sprite = calloc(1, sizeof(*sprite));
	frame_header = calloc(1, sizeof(*frame_header));
	palette = malloc(768);
	
	if (!sprite || !frame_header || !palette)
	{
		printf("Failed to allocate memory for sprite structures\n");
		return (-1);
	}
	
	// Load the sprite file
	result = load_sprite_file(sprite_path, sprite, palette, frame_header, &pixel_data);
	
	if (result != SPRITE_SUCCESS)
	{
		printf("Failed to load sprite '%s': %d\n", sprite_path, result);
		free(sprite);
		free(frame_header);
		free(palette);
		return (-1);
	}
	
	printf("Successfully loaded sprite: %s\n", sprite_path);
	printf("Sprite dimensions: %dx%d\n", frame_header->width, frame_header->height);
	
	// Example: Convert palette indices to RGB colors
	// In a real game, you'd probably upload this to GPU or use it for rendering
	printf("Converting first row of pixels to RGB...\n");
	for (i = 0; i < 10 && i < frame_header->width; i++)
	{
		unsigned char pixel_index = pixel_data[i];  // First row, column i
		unsigned char r = palette[pixel_index * 3];
		unsigned char g = palette[pixel_index * 3 + 1];
		unsigned char b = palette[pixel_index * 3 + 2];
		
		printf("Pixel[%d]: index=%d, RGB=(%d,%d,%d)\n", i, pixel_index, r, g, b);
	}
	
	// Example: Check if sprite is suitable for transparency effects
	if (sprite->text_format == 1)  // ADDITIVE
		printf("This sprite is configured for additive blending\n");
	else if (sprite->text_format == 2)  // INDEXALPHA
		printf("This sprite uses indexed alpha\n");
	else if (sprite->text_format == 3)  // ALPHTEST
		printf("This sprite uses alpha testing\n");
	else
		printf("This sprite uses normal blending\n");
	
	// Example: Simple ASCII art representation (very basic)
	printf("\nASCII representation (first 20x10 pixels):\n");
	for (j = 0; j < 10 && j < frame_header->height; j++)
	{
		for (i = 0; i < 20 && i < frame_header->width; i++)
		{
			unsigned char pixel_index = pixel_data[j * frame_header->width + i];
			// Simple mapping: darker pixels = more dense characters
			if (pixel_index < 64)
				printf("█");
			else if (pixel_index < 128)
				printf("▓");
			else if (pixel_index < 192)
				printf("▒");
			else
				printf("░");
		}
		printf("\n");
	}
	
	// Clean up
	free(sprite);
	free(frame_header);
	free(palette);
	free(pixel_data);
	
	return (0);
}

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