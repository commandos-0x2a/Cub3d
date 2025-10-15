#!/usr/bin/env python3
"""
Image to XPM42 Converter

This script converts any image format supported by Pillow (PIL) to XPM42 format,
which is compatible with MLX42.

XPM42 Format:
- File starts with "!XPM42"
- Header: width height color_count chars_per_pixel mode
- Color entries: character(s) #RRGGBBAA
- Pixel data using the defined characters

Usage:
    python3 converter.py input_image.png [output_file.xpm42]
    python3 converter.py input_image.jpg
    python3 converter.py input_image.bmp output.xpm42
"""

import sys
import os
import argparse
from PIL import Image
import hashlib


class ImageToXPM42Converter:
    def __init__(self):
        self.color_map = {}
        self.char_map = {}
        self.chars_per_pixel = 1
        self.color_count = 0
        
    def generate_character_sequence(self, index):
        """Generate character sequence for pixel representation."""
        chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()_+-=[]{}|;:,.<>?"
        
        if self.chars_per_pixel == 1:
            return chars[index % len(chars)]
        else:
            # For multi-character sequences
            result = ""
            temp_index = index
            for _ in range(self.chars_per_pixel):
                result = chars[temp_index % len(chars)] + result
                temp_index //= len(chars)
            return result
    
    def rgba_to_hex(self, rgba):
        """Convert RGBA tuple to hex string."""
        r, g, b, a = rgba
        return f"#{r:02X}{g:02X}{b:02X}{a:02X}"
    
    def get_unique_colors(self, image):
        """Extract unique colors from image and determine optimal character mapping."""
        unique_colors = set()
        
        # Convert image to RGBA if not already
        if image.mode != 'RGBA':
            image = image.convert('RGBA')
        
        # Get all unique colors
        for pixel in image.getdata():
            unique_colors.add(pixel)
        
        return list(unique_colors)
    
    def determine_chars_per_pixel(self, color_count):
        """Determine how many characters per pixel we need."""
        if color_count <= 94:  # Single printable ASCII characters
            return 1
        elif color_count <= 256:  # Use 2 characters for more colors
            return 2
        else:
            # For very large color counts, we'll need to reduce colors first
            return 2
    
    def reduce_colors(self, image, max_colors=256):
        """Reduce the number of colors in the image if needed."""
        # Convert to RGB first, then quantize, then back to RGBA
        rgb_image = image.convert('RGB')
        quantized = rgb_image.quantize(colors=max_colors, method=Image.Resampling.LANCZOS)
        # Convert back to RGBA
        return quantized.convert('RGBA')
    
    def create_color_mapping(self, unique_colors):
        """Create mapping between colors and character sequences."""
        self.color_count = len(unique_colors)
        self.chars_per_pixel = self.determine_chars_per_pixel(self.color_count)
        
        # If too many colors, we need to reduce them
        if self.color_count > 256:
            print(f"Warning: Image has {self.color_count} colors. This might cause issues.")
            print("Consider reducing image colors or using a different format.")
        
        # Create character mapping
        for i, color in enumerate(unique_colors):
            char_seq = self.generate_character_sequence(i)
            hex_color = self.rgba_to_hex(color)
            self.color_map[color] = char_seq
            self.char_map[char_seq] = hex_color
    
    def convert_to_xpm42(self, input_path, output_path=None):
        """Convert image to XPM42 format."""
        try:
            # Load image
            print(f"Loading image: {input_path}")
            image = Image.open(input_path)
            
            # Get image dimensions
            width, height = image.size
            print(f"Image dimensions: {width}x{height}")
            
            # Convert to RGBA
            if image.mode != 'RGBA':
                image = image.convert('RGBA')
            
            # Get unique colors
            unique_colors = self.get_unique_colors(image)
            original_color_count = len(unique_colors)
            print(f"Original unique colors: {original_color_count}")
            
            # Reduce colors if necessary (use a more conservative limit)
            if len(unique_colors) > 256:
                print("Reducing colors to fit XPM42 format...")
                image = self.reduce_colors(image, 256)
                unique_colors = self.get_unique_colors(image)
                print(f"Reduced to {len(unique_colors)} colors")
            
            # Create color mapping
            self.create_color_mapping(unique_colors)
            
            # Generate output filename if not provided
            if output_path is None:
                base_name = os.path.splitext(input_path)[0]
                output_path = f"{base_name}.xpm42"
            
            # Write XPM42 file
            print(f"Writing XPM42 file: {output_path}")
            with open(output_path, 'w') as f:
                # Write header
                f.write("!XPM42\n")
                f.write(f"{width} {height} {self.color_count} {self.chars_per_pixel} c\n")
                
                # Write color entries
                for char_seq, hex_color in self.char_map.items():
                    f.write(f"{char_seq} {hex_color}\n")
                
                # Write pixel data
                pixel_data = list(image.getdata())
                for y in range(height):
                    line = ""
                    for x in range(width):
                        pixel_index = y * width + x
                        pixel = pixel_data[pixel_index]
                        char_seq = self.color_map[pixel]
                        line += char_seq
                    f.write(line + "\n")
            
            print(f"Successfully converted {input_path} to {output_path}")
            print(f"Final image: {width}x{height}, {self.color_count} colors, {self.chars_per_pixel} chars/pixel")
            
        except Exception as e:
            print(f"Error converting image: {e}")
            sys.exit(1)


def main():
    parser = argparse.ArgumentParser(
        description="Convert any image format to XPM42 format for MLX42",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python3 converter.py texture.png
  python3 converter.py image.jpg output.xpm42
  python3 converter.py photo.bmp custom_name.xpm42
        """
    )
    
    parser.add_argument("input", help="Input image file (PNG, JPG, BMP, etc.)")
    parser.add_argument("output", nargs="?", help="Output XPM42 file (optional)")
    parser.add_argument("-v", "--verbose", action="store_true", help="Verbose output")
    
    args = parser.parse_args()
    
    # Check if input file exists
    if not os.path.exists(args.input):
        print(f"Error: Input file '{args.input}' does not exist.")
        sys.exit(1)
    
    # Check if Pillow is available
    try:
        from PIL import Image
    except ImportError:
        print("Error: Pillow (PIL) is required. Install it with: pip install Pillow")
        sys.exit(1)
    
    # Create converter and convert image
    converter = ImageToXPM42Converter()
    converter.convert_to_xpm42(args.input, args.output)


if __name__ == "__main__":
    main()