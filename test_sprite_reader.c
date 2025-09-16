/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_sprite_reader.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 22:30:00 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/16 09:26:23 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/sprite.h"

/**
 * @brief Get error message from error code
 * @param error_code Error code returned by sprite functions
 * @return String description of the error
 */
const char *get_sprite_error_message(int error_code)
{
	switch (error_code)
	{
		case SPRITE_SUCCESS:
			return "Success";
		case SPRITE_ERROR_NULL_PTR:
			return "Null pointer error";
		case SPRITE_ERROR_FILE_OPEN:
			return "File open error";
		case SPRITE_ERROR_HEADER:
			return "Header read error";
		case SPRITE_ERROR_INVALID:
			return "Invalid sprite format";
		case SPRITE_ERROR_PALETTE:
			return "Palette read error";
		case SPRITE_ERROR_FRAME:
			return "Frame header read error";
		case SPRITE_ERROR_DIMENSIONS:
			return "Invalid frame dimensions";
		case SPRITE_ERROR_MEMORY:
			return "Memory allocation error";
		case SPRITE_ERROR_PIXEL_DATA:
			return "Pixel data read error";
		default:
			return "Unknown error";
	}
}

/**
 * @brief Test sprite loading and display information
 * @param filename Path to sprite file
 * @return 0 on success, -1 on error
 */
int test_sprite_file(const char *filename)
{
	t_sprite_header				*sprite;
	t_sprite_frame_header	*frame_header;
	unsigned char			*palette;
	unsigned char			*pixel_data;
	int						result;
	
	printf("\n=== Testing Sprite File: %s ===\n", filename);
	
	// Allocate structures
	sprite = calloc(1, sizeof(*sprite));
	frame_header = calloc(1, sizeof(*frame_header));
	palette = malloc(768);  // 256 colors * 3 bytes RGB
	
	if (!sprite || !frame_header || !palette)
	{
		printf("Error: Memory allocation failed\n");
		free(sprite);
		free(frame_header);
		free(palette);
		return (-1);
	}
	
	// Load sprite file
	result = load_sprite_file(filename, sprite, palette, frame_header, &pixel_data);
	
	if (result != SPRITE_SUCCESS)
	{
		printf("Error loading sprite: %s (code: %d)\n", get_sprite_error_message(result), result);
		free(sprite);
		free(frame_header);
		free(palette);
		return (-1);
	}
	
	// Display sprite information
	print_sprite_info(sprite);
	print_frame_info(frame_header);
	
	// Display some palette information
	printf("\nPalette Information:\n");
	printf("  First color (RGB): (%d, %d, %d)\n", palette[0], palette[1], palette[2]);
	printf("  Last color (RGB): (%d, %d, %d)\n", palette[765], palette[766], palette[767]);
	
	// Display some pixel data statistics
	if (pixel_data)
	{
		int pixel_count = frame_header->width * frame_header->height;
		int min_pixel = 255, max_pixel = 0;
		int i;
		
		for (i = 0; i < pixel_count; i++)
		{
			if (pixel_data[i] < min_pixel)
				min_pixel = pixel_data[i];
			if (pixel_data[i] > max_pixel)
				max_pixel = pixel_data[i];
		}
		
		printf("\nPixel Data Information:\n");
		printf("  Total pixels: %d\n", pixel_count);
		printf("  Pixel value range: %d - %d\n", min_pixel, max_pixel);
		printf("  First few pixels: %d %d %d %d %d\n", 
			   pixel_data[0], pixel_data[1], pixel_data[2], pixel_data[3], pixel_data[4]);
	}
	
	printf("\n✓ Sprite loaded successfully!\n");
	
	// Cleanup
	free(sprite);
	free(frame_header);
	free(palette);
	free(pixel_data);
	
	return (0);
}

/**
 * @brief Main function to test sprite reader
 */
int main(int argc, char **argv)
{
	int i;
	
	printf("Sprite File Reader Test Program\n");
	printf("===============================\n");
	
	if (argc < 2)
	{
		printf("Usage: %s <sprite_file1> [sprite_file2] ...\n", argv[0]);
		printf("\nExample:\n");
		printf("  %s bonfire.spr\n", argv[0]);
		printf("  %s *.spr\n", argv[0]);
		return (1);
	}
	
	// Test each sprite file provided
	for (i = 1; i < argc; i++)
	{
		if (test_sprite_file(argv[i]) != 0)
		{
			printf("Failed to load sprite: %s\n", argv[i]);
		}
	}
	
	printf("\nTest completed.\n");
	return (0);
}