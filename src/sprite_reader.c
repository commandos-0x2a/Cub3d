/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_reader.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 22:25:00 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/14 23:05:16 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include "../include/sprite.h"

/**
 * @brief Validate sprite header
 * @param sprite Pointer to sprite structure
 * @return 0 if valid, -1 if invalid
 */
int	validate_sprite_header(const t_sprite *sprite)
{
	if (!sprite)
		return (-1);
	
	// Check sprite ID (should be "IDSP" = 0x50534449 in little-endian)
	if (sprite->id != 0x50534449)
		return (-1);
	
	// Check version (should be 2 for HL1 sprites)
	if (sprite->version != 2)
		return (-1);
	
	// Check reasonable bounds
	if (sprite->max_width <= 0 || sprite->max_height <= 0)
		return (-1);
	if (sprite->max_width > 4096 || sprite->max_height > 4096)
		return (-1);
	if (sprite->nb_frame <= 0 || sprite->nb_frame > 1000)
		return (-1);
	if (sprite->palette_color_count != 256)
		return (-1);
	
	// Check sprite type is valid (0-4)
	if (sprite->type < 0 || sprite->type > 4)
		return (-1);
	
	// Check text format is valid (0-3)
	if (sprite->text_format < 0 || sprite->text_format > 3)
		return (-1);
	
	return (0);
}

/**
 * @brief Load complete sprite from file
 * @param filename Path to sprite file
 * @param sprite Pointer to sprite structure to fill
 * @param palette Buffer for palette data (768 bytes)
 * @param frame_header Pointer to frame header structure to fill
 * @param pixel_data Buffer for pixel data (allocated inside function)
 * @return 0 on success, negative error code on failure
 */
int	load_sprite_file(const char *filename, t_sprite *sprite, 
					unsigned char *palette, t_sprite_frame_header *frame_header,
					unsigned char **pixel_data)
{
	int	fd;
	int	pixel_data_size;
	
	if (!filename || !sprite || !palette || !frame_header || !pixel_data)
		return (-1);
	
	// Open file
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (-2);  // File open error
	

	if (read(fd, sprite, sizeof(*sprite)) != sizeof(*sprite))
	{
		close(fd);
		return (-3);  // Header read error
	}

	// Validate sprite header
	if (validate_sprite_header(sprite) != 0)
	{
		close(fd);
		return (-4);  // Invalid sprite format
	}

	// Read 256 colors * 3 bytes (RGB) = 768 bytes
	if (read(fd, palette, 768) != 768)
	{
		close(fd);
		return (-5);  // Palette read error
	}

	// Read frame header fields in order
	// For simplicity, read only the first frame
	if (read(fd, frame_header, sizeof(*frame_header)) != sizeof(*frame_header))
	{
		close(fd);
		return (-6);  // Frame header read error
	}
	
	// Validate frame dimensions
	if (frame_header->width <= 0 || frame_header->height <= 0 ||
		frame_header->width > 4096 || frame_header->height > 4096)
	{
		close(fd);
		return (-7);  // Invalid frame dimensions
	}
	
	// Allocate pixel data
	pixel_data_size = frame_header->width * frame_header->height;
	*pixel_data = malloc(pixel_data_size);
	if (!*pixel_data)
	{
		close(fd);
		return (-8);  // Memory allocation error
	}
	
	int expected_size = frame_header->width * frame_header->height;
	if (read(fd, *pixel_data, expected_size) != expected_size)
	{
		free(*pixel_data);
		*pixel_data = NULL;
		close(fd);
		return (-9);  // Pixel data read error
	}
	close(fd);
	return (0);  // Success
}

/**
 * @brief Print sprite information for debugging
 * @param sprite Pointer to sprite structure
 */
void	print_sprite_info(const t_sprite *sprite)
{
	if (!sprite)
		return;
	
	printf("Sprite Information:\n");
	printf("  ID: 0x%08X\n", sprite->id);
	printf("  Version: %d\n", sprite->version);
	printf("  Type: %d\n", sprite->type);
	printf("  Text Format: %d\n", sprite->text_format);
	printf("  Bounding Radius: %.2f\n", sprite->bounding_radius);
	printf("  Max Dimensions: %dx%d\n", sprite->max_width, sprite->max_height);
	printf("  Number of Frames: %d\n", sprite->nb_frame);
	printf("  Beam Length: %.2f\n", sprite->beam_length);
	printf("  Sync Type: %d\n", sprite->synch_type);
	printf("  Palette Colors: %d\n", sprite->palette_color_count);
}

/**
 * @brief Print frame header information for debugging
 * @param frame_header Pointer to frame header structure
 */
void	print_frame_info(const t_sprite_frame_header *frame_header)
{
	if (!frame_header)
		return;
	
	printf("Frame Information:\n");
	printf("  Group: %d\n", frame_header->group);
	printf("  Origin: (%d, %d)\n", frame_header->originX, frame_header->originY);
	printf("  Dimensions: %dx%d\n", frame_header->width, frame_header->height);
}
