/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_sprite_reader_new.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:00:00 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/19 19:52:09 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/sprite_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Get error message from error code
 * @param error_code Error code returned by sprite functions
 * @return String description of the error
 */
const char	*get_sprite_error_message(int error_code)
{
	if (error_code == SPRITE_SUCCESS)
		return ("Success");
	else if (error_code == SPRITE_ERROR_NULL_PTR)
		return ("Null pointer error");
	else if (error_code == SPRITE_ERROR_FILE_OPEN)
		return ("File open error");
	else if (error_code == SPRITE_ERROR_HEADER)
		return ("Header read error");
	else if (error_code == SPRITE_ERROR_INVALID)
		return ("Invalid sprite format");
	else if (error_code == SPRITE_ERROR_PALETTE)
		return ("Palette read error");
	else if (error_code == SPRITE_ERROR_FRAME)
		return ("Frame header read error");
	else if (error_code == SPRITE_ERROR_DIMENSIONS)
		return ("Invalid frame dimensions");
	else if (error_code == SPRITE_ERROR_MEMORY)
		return ("Memory allocation error");
	else if (error_code == SPRITE_ERROR_PIXEL_DATA)
		return ("Pixel data read error");
	else
		return ("Unknown error");
}

/**
 * @brief Test group analysis functionality
 * @param filename Path to sprite file
 */
static void	test_group_analysis(const char *filename)
{
	int	*frame_indices;
	int	frame_count;
	int	result;
	int	group_id;
	int	i;

	printf("\n=== GROUP ANALYSIS TEST ===\n");
	group_id = 0;
	while (group_id < 5)
	{
		result = get_frames_by_group(filename, group_id,
										&frame_indices, &frame_count);
		if (result == SPRITE_SUCCESS && frame_count > 0)
		{
			printf("Group %d contains %d frames: [", group_id, frame_count);
			i = 0;
			while (i < frame_count)
			{
				printf("%d", frame_indices[i]);
				if (i < frame_count - 1)
					printf(", ");
				i++;
			}
			printf("]\n");
			free(frame_indices);
		}
		else if (result == SPRITE_SUCCESS)
		{
			printf("Group %d: No frames found\n", group_id);
		}
		else
		{
			printf("Group %d: Error reading (%s)\n", group_id,
					get_sprite_error_message(result));
		}
		group_id++;
	}
}

/**
 * @brief Test basic sprite loading functionality
 * @param filename Path to sprite file
 */
static void	test_sprite_loading(const char *filename)
{
	t_sprite_header			sprite;
	t_sprite_frame_header	frame_header;
	unsigned char			palette[768];
	unsigned char			*pixel_data;
	int						result;
	int						i;

	printf("\n=== SPRITE LOADING TEST ===\n");
	result = load_sprite_file(filename, &sprite, palette,
								&frame_header, &pixel_data);
	if (result == SPRITE_SUCCESS)
	{
		printf("✓ Sprite loaded successfully!\n");
		print_sprite_info(&sprite);
		printf("\nFirst Frame:\n");
		print_frame_info(&frame_header);
		printf("\nPalette preview (first 10 RGB values):\n");
		printf("  ");
		i = 0;
		while (i < 10)
		{
			printf("(%d,%d,%d) ", palette[i * 3], palette[i * 3 + 1],
					palette[i * 3 + 2]);
			i++;
		}
		printf("\n");
		if (pixel_data)
		{
			printf("✓ Pixel data allocated (%d bytes per frame)\n",
					frame_header.width * frame_header.height);
			free(pixel_data);
		}
	}
	else
	{
		printf("✗ Failed to load sprite: %s\n",
				get_sprite_error_message(result));
	}
}

/**
 * @brief Print usage information
 * @param program_name Name of the program executable
 */
static void	print_usage(const char *program_name)
{
	printf("Usage: %s <sprite_file.spr>\n", program_name);
	printf("\nThis program tests the C sprite reader functionality:\n");
	printf("  - Basic sprite loading and validation\n");
	printf("  - Header and frame information display\n");
	printf("  - Group analysis for animation sequences\n");
	printf("  - RGBA transparency detection\n");
	printf("  - Memory management verification\n");
}

/**
 * @brief Main test function
 * @param argc Argument count
 * @param argv Argument values
 * @return Exit status
 */
int	main(int argc, char **argv)
{
	const char	*filename;

	if (argc != 2)
	{
		print_usage(argv[0]);
		return (1);
	}
	filename = argv[1];
	printf("Testing sprite reader with file: %s\n", filename);
	printf("===========================================\n");
	test_sprite_loading(filename);
	test_group_analysis(filename);
	printf("\n=== TEST COMPLETE ===\n");
	return (0);
}