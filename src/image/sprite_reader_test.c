/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite_reader_test.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yaltayeh <yaltayeh@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 17:00:00 by yaltayeh          #+#    #+#             */
/*   Updated: 2025/09/19 19:52:09 by yaltayeh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sprite_test.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

/**
 * @brief Read exactly n bytes from file descriptor
 * @param fd File descriptor to read from
 * @param buffer Buffer to store data
 * @param size Number of bytes to read
 * @return SPRITE_SUCCESS on success, error code on failure
 */
static int	read_exact(int fd, void *buffer, size_t size)
{
	ssize_t	bytes_read;
	ssize_t	total_read;
	char	*buf_ptr;

	if (!buffer)
		return (SPRITE_ERROR_NULL_PTR);
	buf_ptr = (char *)buffer;
	total_read = 0;
	while (total_read < (ssize_t)size)
	{
		bytes_read = read(fd, buf_ptr + total_read, size - total_read);
		if (bytes_read <= 0)
			return (SPRITE_ERROR_HEADER);
		total_read += bytes_read;
	}
	return (SPRITE_SUCCESS);
}

/**
 * @brief Validate sprite header magic and version
 * @param sprite Pointer to sprite header structure
 * @return SPRITE_SUCCESS if valid, SPRITE_ERROR_INVALID if not
 */
int	validate_sprite_header(const t_sprite_header *sprite)
{
	const int	expected_id = 0x50534449;
	const int	expected_version = 2;

	if (!sprite)
		return (SPRITE_ERROR_NULL_PTR);
	if (sprite->id != expected_id)
		return (SPRITE_ERROR_INVALID);
	if (sprite->version != expected_version)
		return (SPRITE_ERROR_INVALID);
	if (sprite->palette_color_count != 256)
		return (SPRITE_ERROR_INVALID);
	if (sprite->nb_frame <= 0 || sprite->nb_frame > 1000)
		return (SPRITE_ERROR_INVALID);
	return (SPRITE_SUCCESS);
}

/**
 * @brief Check if sprite uses transparency (INDEXALPHA format)
 * @param sprite Pointer to sprite header structure
 * @return 1 if transparent, 0 if not
 */
static int	sprite_has_transparency(const t_sprite_header *sprite)
{
	const int	indexalpha_format = 2;

	if (!sprite)
		return (0);
	return (sprite->text_format == indexalpha_format);
}

/**
 * @brief Read sprite header from file
 * @param fd File descriptor
 * @param sprite Pointer to sprite header structure
 * @return SPRITE_SUCCESS on success, error code on failure
 */
static int	read_sprite_header(int fd, t_sprite_header *sprite)
{
	int	result;

	if (!sprite)
		return (SPRITE_ERROR_NULL_PTR);
	result = read_exact(fd, sprite, sizeof(t_sprite_header));
	if (result != SPRITE_SUCCESS)
		return (result);
	return (validate_sprite_header(sprite));
}

/**
 * @brief Read palette data from sprite file
 * @param fd File descriptor
 * @param palette Buffer to store palette (must be 768 bytes)
 * @return SPRITE_SUCCESS on success, error code on failure
 */
static int	read_sprite_palette(int fd, unsigned char *palette)
{
	const size_t	palette_size = 768;

	if (!palette)
		return (SPRITE_ERROR_NULL_PTR);
	return (read_exact(fd, palette, palette_size));
}

/**
 * @brief Read frame header from sprite file
 * @param fd File descriptor
 * @param frame Pointer to frame header structure
 * @return SPRITE_SUCCESS on success, error code on failure
 */
static int	read_frame_header(int fd, t_sprite_frame_header *frame)
{
	int	result;

	if (!frame)
		return (SPRITE_ERROR_NULL_PTR);
	result = read_exact(fd, frame, sizeof(t_sprite_frame_header));
	if (result != SPRITE_SUCCESS)
		return (result);
	if (frame->width <= 0 || frame->height <= 0)
		return (SPRITE_ERROR_DIMENSIONS);
	if (frame->width > 2048 || frame->height > 2048)
		return (SPRITE_ERROR_DIMENSIONS);
	return (SPRITE_SUCCESS);
}

/**
 * @brief Allocate memory for pixel data
 * @param frame_count Number of frames
 * @param frame_width Width of each frame
 * @param frame_height Height of each frame
 * @return Pointer to allocated memory, NULL on failure
 */
static unsigned char	*alloc_pixel_data(int frame_count, int frame_width,
														int frame_height)
{
	size_t			total_pixels;
	unsigned char	*pixel_data;

	if (frame_count <= 0 || frame_width <= 0 || frame_height <= 0)
		return (NULL);
	total_pixels = (size_t)frame_count * frame_width * frame_height;
	if (total_pixels > SIZE_MAX / sizeof(unsigned char))
		return (NULL);
	pixel_data = malloc(total_pixels * sizeof(unsigned char));
	return (pixel_data);
}

/**
 * @brief Read pixel data for all frames
 * @param fd File descriptor
 * @param sprite Sprite header with frame count
 * @param frame_width Width of frames
 * @param frame_height Height of frames
 * @param pixel_data Buffer to store pixel data
 * @return SPRITE_SUCCESS on success, error code on failure
 */
static int	read_pixel_data(int fd, const t_sprite_header *sprite,
								int frame_width, int frame_height,
								unsigned char *pixel_data)
{
	size_t	frame_size;
	size_t	total_size;
	int		result;

	if (!sprite || !pixel_data)
		return (SPRITE_ERROR_NULL_PTR);
	frame_size = (size_t)frame_width * frame_height;
	total_size = frame_size * sprite->nb_frame;
	result = read_exact(fd, pixel_data, total_size);
	return (result);
}

/**
 * @brief Load complete sprite file
 * @param filename Path to sprite file
 * @param sprite Pointer to sprite header structure
 * @param palette Buffer for palette data (768 bytes)
 * @param frame_header Pointer to frame header structure
 * @param pixel_data Pointer to store allocated pixel data
 * @return SPRITE_SUCCESS on success, error code on failure
 */
int	load_sprite_file(const char *filename, t_sprite_header *sprite,
						unsigned char *palette, t_sprite_frame_header *frame_header,
						unsigned char **pixel_data)
{
	int	fd;
	int	result;

	if (!filename || !sprite || !palette || !frame_header || !pixel_data)
		return (SPRITE_ERROR_NULL_PTR);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (SPRITE_ERROR_FILE_OPEN);
	result = read_sprite_header(fd, sprite);
	if (result != SPRITE_SUCCESS)
	{
		close(fd);
		return (result);
	}
	result = read_sprite_palette(fd, palette);
	if (result != SPRITE_SUCCESS)
	{
		close(fd);
		return (result);
	}
	result = read_frame_header(fd, frame_header);
	if (result != SPRITE_SUCCESS)
	{
		close(fd);
		return (result);
	}
	*pixel_data = alloc_pixel_data(sprite->nb_frame, frame_header->width,
									frame_header->height);
	if (!*pixel_data)
	{
		close(fd);
		return (SPRITE_ERROR_MEMORY);
	}
	result = read_pixel_data(fd, sprite, frame_header->width,
								frame_header->height, *pixel_data);
	close(fd);
	return (result);
}

/**
 * @brief Get frame data by group ID
 * @param filename Path to sprite file
 * @param group_id Target group ID
 * @param frame_indices Array to store frame indices (allocated by function)
 * @param frame_count Pointer to store number of frames found
 * @return SPRITE_SUCCESS on success, error code on failure
 */
int	get_frames_by_group(const char *filename, int group_id,
							int **frame_indices, int *frame_count)
{
	int						fd;
	t_sprite_header			sprite;
	t_sprite_frame_header	frame;
	unsigned char			palette[768];
	int						*indices;
	int						found_count;
	int						i;
	int						result;

	if (!filename || !frame_indices || !frame_count)
		return (SPRITE_ERROR_NULL_PTR);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (SPRITE_ERROR_FILE_OPEN);
	result = read_sprite_header(fd, &sprite);
	if (result != SPRITE_SUCCESS)
	{
		close(fd);
		return (result);
	}
	result = read_sprite_palette(fd, palette);
	if (result != SPRITE_SUCCESS)
	{
		close(fd);
		return (result);
	}
	indices = malloc(sizeof(int) * sprite.nb_frame);
	if (!indices)
	{
		close(fd);
		return (SPRITE_ERROR_MEMORY);
	}
	found_count = 0;
	i = 0;
	while (i < sprite.nb_frame)
	{
		result = read_frame_header(fd, &frame);
		if (result != SPRITE_SUCCESS)
		{
			free(indices);
			close(fd);
			return (result);
		}
		if (frame.group == group_id)
			indices[found_count++] = i;
		lseek(fd, (off_t)frame.width * frame.height, SEEK_CUR);
		i++;
	}
	close(fd);
	*frame_indices = indices;
	*frame_count = found_count;
	return (SPRITE_SUCCESS);
}

/**
 * @brief Print sprite header information
 * @param sprite Pointer to sprite header structure
 */
void	print_sprite_info(const t_sprite_header *sprite)
{
	if (!sprite)
		return ;
	printf("Sprite Information:\n");
	printf("  ID: %c%c%c%c\n", (sprite->id & 0xFF),
			((sprite->id >> 8) & 0xFF), ((sprite->id >> 16) & 0xFF),
			((sprite->id >> 24) & 0xFF));
	printf("  Version: %d\n", sprite->version);
	printf("  Type: %d\n", sprite->type);
	printf("  Text Format: %d", sprite->text_format);
	if (sprite_has_transparency(sprite))
		printf(" (INDEXALPHA - Transparency supported)\n");
	else
		printf(" (No transparency)\n");
	printf("  Dimensions: %dx%d\n", sprite->max_width, sprite->max_height);
	printf("  Frames: %d\n", sprite->nb_frame);
	printf("  Palette Colors: %d\n", sprite->palette_color_count);
}

/**
 * @brief Print frame header information
 * @param frame_header Pointer to frame header structure
 */
void	print_frame_info(const t_sprite_frame_header *frame_header)
{
	if (!frame_header)
		return ;
	printf("Frame Information:\n");
	printf("  Group: %d\n", frame_header->group);
	printf("  Origin: (%d, %d)\n", frame_header->originX, frame_header->originY);
	printf("  Size: %dx%d\n", frame_header->width, frame_header->height);
}