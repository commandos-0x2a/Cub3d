
#!/usr/bin/env python3
"""
Image to Sprite Converter
Converts any image format to .spr sprite format based on Half-Life 1 sprite specification.
"""

import struct
import sys
import os
from PIL import Image
import argparse

class SpriteConverter:
    def __init__(self):
        self.SPRITE_ID = b'IDSP'  # Format ID
        self.VERSION = 2          # HL1 sprite version
        self.TYPE_VP_PARALLEL_UPRIGHT = 0
        self.TYPE_FACING_UPRIGHT = 1
        self.TYPE_VP_PARALLEL = 2
        self.TYPE_ORIENTED = 3
        self.TYPE_VP_PARALLEL_ORIENTED = 4
        
        self.TEXT_NORMAL = 0
        self.TEXT_ADDITIVE = 1
        self.TEXT_INDEXALPHA = 2
        self.TEXT_ALPHTEST = 3
        
    def create_palette(self, image):
        """Create a 256-color palette from the image."""
        # Convert to RGB if not already
        if image.mode != 'RGB':
            image = image.convert('RGB')
            
        # Quantize to 256 colors with palette
        quantized = image.quantize(colors=256, method=Image.Quantize.MEDIANCUT)
        palette = quantized.getpalette()
        
        # Ensure we have exactly 256 colors (768 bytes for RGB)
        if len(palette) < 768:
            palette.extend([0] * (768 - len(palette)))
        elif len(palette) > 768:
            palette = palette[:768]
            
        return palette, quantized
    
    def convert_image_to_sprite(self, input_path, output_path=None, 
                              sprite_type=2, text_format=0, 
                              frame_width=None, frame_height=None):
        """Convert an image to sprite format."""
        
        if not os.path.exists(input_path):
            raise FileNotFoundError(f"Input file not found: {input_path}")
            
        # Open and process image
        try:
            image = Image.open(input_path)
        except Exception as e:
            raise ValueError(f"Cannot open image: {e}")
            
        # Set output path if not provided
        if output_path is None:
            base_name = os.path.splitext(input_path)[0]
            output_path = f"{base_name}.spr"
            
        # Handle frame dimensions
        if frame_width is None:
            frame_width = image.width
        if frame_height is None:
            frame_height = image.height
            
        # Create palette and quantized image
        palette, quantized_image = self.create_palette(image)
        
        # Calculate sprite properties
        bounding_radius = max(frame_width, frame_height) / 2.0
        max_width = frame_width
        max_height = frame_height
        
        # For simplicity, create single frame sprite
        nb_frames = 1
        
        # Write sprite file
        with open(output_path, 'wb') as f:
            # Write sprite header
            f.write(self.SPRITE_ID)  # 4 bytes
            f.write(struct.pack('<I', self.VERSION))  # 4 bytes
            f.write(struct.pack('<I', sprite_type))  # 4 bytes
            f.write(struct.pack('<I', text_format))  # 4 bytes
            f.write(struct.pack('<f', bounding_radius))  # 4 bytes
            f.write(struct.pack('<I', max_width))  # 4 bytes
            f.write(struct.pack('<I', max_height))  # 4 bytes
            f.write(struct.pack('<I', nb_frames))  # 4 bytes
            f.write(struct.pack('<f', 0.0))  # beam_length - 4 bytes
            f.write(struct.pack('<I', 0))  # synch_type - 4 bytes
            f.write(struct.pack('<H', 256))  # palette_color_count - 2 bytes
            
            # Write palette (256 colors * 3 bytes RGB = 768 bytes)
            for i in range(0, 768, 3):
                r = palette[i] if i < len(palette) else 0
                g = palette[i+1] if i+1 < len(palette) else 0
                b = palette[i+2] if i+2 < len(palette) else 0
                f.write(struct.pack('BBB', r, g, b))
            
            # Write frame header
            f.write(struct.pack('<I', 0))  # group
            f.write(struct.pack('<i', 0))  # originX
            f.write(struct.pack('<i', 0))  # originY
            f.write(struct.pack('<I', frame_width))  # width
            f.write(struct.pack('<I', frame_height))  # height
            
            # Resize image to frame dimensions if needed
            if quantized_image.size != (frame_width, frame_height):
                quantized_image = quantized_image.resize((frame_width, frame_height), Image.Resampling.LANCZOS)
            
            # Write pixel data
            pixel_data = list(quantized_image.getdata())
            for pixel in pixel_data:
                f.write(struct.pack('B', pixel))
                
        print(f"Sprite saved to: {output_path}")
        print(f"Dimensions: {frame_width}x{frame_height}")
        print(f"Colors: 256")
        return output_path

def main():
    parser = argparse.ArgumentParser(description='Convert images to .spr sprite format')
    parser.add_argument('input', help='Input image file path')
    parser.add_argument('-o', '--output', help='Output sprite file path (optional)')
    parser.add_argument('-w', '--width', type=int, help='Frame width (default: image width)')
    parser.add_argument('--height', type=int, help='Frame height (default: image height)')
    parser.add_argument('-t', '--type', type=int, default=2, choices=[0,1,2,3,4],
                       help='Sprite type: 0=VP_PARALLEL_UPRIGHT, 1=FACING_UPRIGHT, 2=VP_PARALLEL, 3=ORIENTED, 4=VP_PARALLEL_ORIENTED')
    parser.add_argument('-f', '--format', type=int, default=0, choices=[0,1,2,3],
                       help='Text format: 0=NORMAL, 1=ADDITIVE, 2=INDEXALPHA, 3=ALPHTEST')
    
    # Handle the case where -h conflicts with help
    args = parser.parse_args()
    
    converter = SpriteConverter()
    
    try:
        converter.convert_image_to_sprite(
            input_path=args.input,
            output_path=args.output,
            sprite_type=args.type,
            text_format=args.format,
            frame_width=args.width,
            frame_height=args.height
        )
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)

def interactive_mode():
    """Interactive mode for user input."""
    converter = SpriteConverter()
    
    print("=== Image to Sprite Converter ===")
    print()
    
    # Get input file
    while True:
        file_path = input("Enter image file path: ").strip()
        if os.path.exists(file_path):
            break
        print("File not found. Please try again.")
    
    # Get optional frame dimensions
    try:
        image = Image.open(file_path)
        print(f"Original image size: {image.width}x{image.height}")
        
        frame_width_input = input(f"Frame width (default: {image.width}): ").strip()
        frame_width = int(frame_width_input) if frame_width_input else image.width
        
        frame_height_input = input(f"Frame height (default: {image.height}): ").strip()
        frame_height = int(frame_height_input) if frame_height_input else image.height
        
    except Exception as e:
        print(f"Error reading image: {e}")
        return
    
    # Get output path
    base_name = os.path.splitext(file_path)[0]
    default_output = f"{base_name}.spr"
    output_path = input(f"Output path (default: {default_output}): ").strip()
    if not output_path:
        output_path = default_output
    
    # Convert
    try:
        converter.convert_image_to_sprite(
            input_path=file_path,
            output_path=output_path,
            frame_width=frame_width,
            frame_height=frame_height
        )
        print("Conversion completed successfully!")
    except Exception as e:
        print(f"Error during conversion: {e}")

if __name__ == "__main__":
    if len(sys.argv) == 1:
        # No arguments provided, run interactive mode
        interactive_mode()
    else:
        # Command line arguments provided
        main()