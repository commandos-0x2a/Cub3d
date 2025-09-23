# C Sprite Reader - Complete Implementation

## Overview

This is a comprehensive C sprite reader implementation that handles Half-Life 1 compatible sprites with full support for:

- **RGBA Transparency**: Automatic detection of INDEXALPHA format (format 2) sprites
- **Group Support**: Frame organization for animation sequences
- **42 Norm Compliance**: All code follows 42 School coding standards
- **Memory Safety**: Proper allocation/deallocation with error handling
- **MLX42 Integration**: Functions to create textures for game rendering

## Files Structure

```
include/sprite.h                    # Main header with MLX42 support
include/sprite_test.h               # Test header without MLX42 dependency
src/image/sprite_reader.c           # Full implementation with MLX42
src/image/sprite_reader_test.c      # Test implementation without MLX42
scripts/test_sprite_reader_new.c    # Comprehensive test program
sprite_test.mk                      # Test Makefile
```

## Core Functions

### Basic Sprite Loading
```c
int load_sprite_file(const char *filename, t_sprite_header *sprite,
                     unsigned char *palette, t_sprite_frame_header *frame_header,
                     unsigned char **pixel_data);
```
Loads complete sprite file including header, palette, first frame header, and all pixel data.

### Group Analysis
```c
int get_frames_by_group(const char *filename, int group_id,
                        int **frame_indices, int *frame_count);
```
Returns array of frame indices belonging to specific group for animation sequences.

### MLX Texture Creation
```c
mlx_texture_t *create_sprite_texture(const unsigned char *pixel_data,
                                     const unsigned char *palette,
                                     int width, int height, int has_alpha,
                                     mlx_t *mlx);
```
Creates MLX42 texture with proper RGBA conversion and transparency handling.

### Validation & Info
```c
int validate_sprite_header(const t_sprite_header *sprite);
void print_sprite_info(const t_sprite_header *sprite);
void print_frame_info(const t_sprite_frame_header *frame_header);
```

## Building & Testing

### Test Build (without MLX42)
```bash
make -f sprite_test.mk
./test_sprite_reader <sprite_file.spr>
```

### Main Project Build (with MLX42)
```bash
make  # Includes sprite_reader.c in main build
```

## Usage Examples

### Basic Loading
```c
t_sprite_header sprite;
t_sprite_frame_header frame;
unsigned char palette[768];
unsigned char *pixel_data;

int result = load_sprite_file("sprite.spr", &sprite, palette, &frame, &pixel_data);
if (result == SPRITE_SUCCESS) {
    print_sprite_info(&sprite);
    print_frame_info(&frame);
    free(pixel_data);
}
```

### Group Analysis
```c
int *frame_indices;
int frame_count;

int result = get_frames_by_group("sprite.spr", 0, &frame_indices, &frame_count);
if (result == SPRITE_SUCCESS && frame_count > 0) {
    printf("Group 0 has %d frames\n", frame_count);
    for (int i = 0; i < frame_count; i++) {
        printf("Frame %d\n", frame_indices[i]);
    }
    free(frame_indices);
}
```

### MLX Texture Creation
```c
mlx_t *mlx = mlx_init(800, 600, "Game", false);
mlx_texture_t *texture = create_sprite_texture(pixel_data, palette,
                                               frame.width, frame.height,
                                               sprite.text_format == 2, mlx);
if (texture) {
    // Use texture for rendering
}
```

## Transparency Support

The reader automatically detects INDEXALPHA sprites (format 2) where palette index 255 represents transparent pixels. RGBA conversion sets alpha=0 for transparent pixels, alpha=255 for opaque pixels.

## Group System

Groups enable complex animation sequences:
- **Group 0**: Often idle/default animation
- **Group 1+**: Movement, attack, death animations
- Multiple frames per group for smooth animation

## Error Handling

All functions return error codes:
- `SPRITE_SUCCESS`: Operation completed successfully
- `SPRITE_ERROR_FILE_OPEN`: Cannot open file
- `SPRITE_ERROR_INVALID`: Invalid sprite format
- `SPRITE_ERROR_MEMORY`: Memory allocation failed
- `SPRITE_ERROR_DIMENSIONS`: Invalid frame dimensions

## Test Results

Successfully tested with:
- **bonfire.spr**: 6-frame animated sprite with transparency (INDEXALPHA)
- **north_texture.spr**: Single-frame texture without transparency
- **directions_multigroup.spr**: 15-frame sprite with 4 different groups

## Integration with Cub3D

The sprite reader integrates with the Cub3D raycasting engine:
1. Load sprites during initialization
2. Create MLX textures for rendering
3. Use group analysis for animation state management
4. Handle transparency for overlay effects

## 42 Norm Compliance

All code follows 42 School standards:
- Functions under 25 lines
- Maximum 80 characters per line
- Proper header comments
- Consistent naming conventions
- No forbidden functions