# Image to XPM42 Converter

This utility converts any image format supported by Pillow (PNG, JPG, BMP, GIF, etc.) to XPM42 format, which is compatible with MLX42.

## Requirements

- Python 3.x
- Pillow (PIL) library

Install Pillow if not already installed:
```bash
pip install Pillow
```

## Usage

### Basic Usage
```bash
# Convert with automatic output filename
python3 converter.py input_image.png
# Output: input_image.xpm42

# Convert with custom output filename
python3 converter.py input_image.jpg custom_output.xmp42

# Show help
python3 converter.py --help
```

### Examples
```bash
# Convert a texture file
python3 converter.py textures/north_texture.png

# Convert with specific output name
python3 converter.py textures/player_sprite.png sprites/player.xpm42

# Convert multiple files (using shell loop)
for img in textures/*.png; do python3 converter.py "$img"; done
```

## Features

- **Automatic Color Reduction**: Images with too many colors are automatically reduced to fit XPM42 limitations
- **Optimal Character Mapping**: Uses single characters when possible, multi-character sequences when needed
- **RGBA Support**: Preserves transparency information
- **Multiple Input Formats**: Supports PNG, JPG, BMP, GIF, and other Pillow-supported formats

## XPM42 Format

XPM42 is a custom format for MLX42 with the following structure:

```
!XPM42                    # File declaration
width height colors cpp mode  # Header: width, height, color count, chars per pixel, mode
char1 #RRGGBBAA          # Color entries (RGBA format)
char2 #RRGGBBAA
...
pixel_data_line_1        # Pixel data using defined characters
pixel_data_line_2
...
```

## Color Limitations

- **Single Character Mode**: Up to 94 colors (using printable ASCII characters)
- **Two Character Mode**: Up to 256 colors (automatic color reduction applied)
- Images with more colors are automatically quantized to 256 colors

## Technical Details

- Uses FNV-1a hash for character-to-color mapping (compatible with MLX42)
- Preserves transparency with RGBA format
- Optimizes characters per pixel based on color count
- Handles large images efficiently with memory-conscious processing

## Integration with MLX42

The generated XPM42 files can be loaded directly in MLX42:

```c
#include "MLX42/MLX42.h"

// Load XPM42 file
xpm_t* xpm = mlx_load_xpm42("texture.xpm42");
if (!xpm) {
    // Handle error
    puts(mlx_strerror(mlx_errno));
    return;
}

// Convert to displayable image
mlx_image_t* img = mlx_texture_to_image(mlx, &xpm->texture);
if (!img) {
    // Handle error
    puts(mlx_strerror(mlx_errno));
    mlx_delete_xpm42(xpm);
    return;
}

// Display the image
mlx_image_to_window(mlx, img, x, y);

// Cleanup
mlx_delete_image(mlx, img);
mlx_delete_xpm42(xpm);
```

## Troubleshooting

### "Pillow is required" Error
Install Pillow:
```bash
pip install Pillow
# or
pip3 install Pillow
```

### "Too many colors" Warning
The converter automatically reduces colors, but you can manually reduce them first:
```bash
# Use image editing software to reduce colors
# Or use ImageMagick:
convert input.png -colors 256 output.png
python3 converter.py output.png
```

### Large File Sizes
XPM42 files can be large for complex images. Consider:
- Reducing image dimensions
- Using simpler images with fewer colors
- Converting to PNG first if the image has too many colors

## Examples in Project

```bash
# Convert all texture files
python3 converter.py textures/north_texture.png
python3 converter.py textures/south_texture.png
python3 converter.py textures/east_texture.png
python3 converter.py textures/west_texture.png

# Convert sprite files
python3 converter.py textures/bonfire.png
python3 converter.py textures/open_door.png
python3 converter.py textures/closed_door.png
```

Generated XPM42 files can then be used in your Cub3D project instead of PNG files for better MLX42 compatibility.
