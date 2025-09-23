
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
        
    def create_palette(self, image, preserve_transparency=True):
        """Create a 256-color palette from the image with optional transparency support."""
        has_transparency = False
        transparency_index = 255  # Reserve last color for transparency
        
        # Check if image has transparency
        if image.mode in ('RGBA', 'LA') or 'transparency' in image.info:
            has_transparency = True
            
        # Convert to appropriate mode for palette generation
        if has_transparency and preserve_transparency:
            if image.mode != 'RGBA':
                image = image.convert('RGBA')
            # Create a version without fully transparent pixels for palette generation
            palette_image = Image.new('RGB', image.size, (0, 0, 0))
            for x in range(image.width):
                for y in range(image.height):
                    pixel = image.getpixel((x, y))
                    if len(pixel) >= 4 and pixel[3] > 0:  # Not fully transparent
                        palette_image.putpixel((x, y), pixel[:3])
            
            # Quantize to 255 colors (reserve one for transparency)
            quantized = palette_image.quantize(colors=255, method=Image.Quantize.MEDIANCUT)
            palette = quantized.getpalette()
        else:
            # Standard RGB handling
            if image.mode != 'RGB':
                image = image.convert('RGB')
            quantized = image.quantize(colors=256, method=Image.Quantize.MEDIANCUT)
            palette = quantized.getpalette()
        
        # Ensure we have exactly 256 colors (768 bytes for RGB)
        if len(palette) < 768:
            palette.extend([0] * (768 - len(palette)))
        elif len(palette) > 768:
            palette = palette[:768]
            
        # Set transparency color (typically magenta for sprite transparency)
        if has_transparency and preserve_transparency:
            palette[transparency_index * 3:transparency_index * 3 + 3] = [255, 0, 255]  # Magenta
            
        return palette, quantized, has_transparency, transparency_index
    
    def extract_frames_from_image(self, image, frame_width, frame_height, 
                                 frames_layout='auto', frames_count=None):
        """Extract multiple frames from an image."""
        frames = []
        
        if frames_layout == 'single':
            # Single frame - resize if needed
            if image.size != (frame_width, frame_height):
                image = image.resize((frame_width, frame_height), Image.Resampling.LANCZOS)
            frames.append(image)
            
        elif frames_layout == 'horizontal':
            # Horizontal strip (frames arranged left to right)
            if frames_count is None:
                frames_count = image.width // frame_width
            
            for i in range(frames_count):
                left = i * frame_width
                right = left + frame_width
                if right > image.width:
                    break
                    
                frame = image.crop((left, 0, right, image.height))
                if frame.height != frame_height:
                    frame = frame.resize((frame_width, frame_height), Image.Resampling.LANCZOS)
                frames.append(frame)
                
        elif frames_layout == 'vertical':
            # Vertical strip (frames arranged top to bottom)
            if frames_count is None:
                frames_count = image.height // frame_height
            
            for i in range(frames_count):
                top = i * frame_height
                bottom = top + frame_height
                if bottom > image.height:
                    break
                    
                frame = image.crop((0, top, image.width, bottom))
                if frame.width != frame_width:
                    frame = frame.resize((frame_width, frame_height), Image.Resampling.LANCZOS)
                frames.append(frame)
                
        elif frames_layout == 'grid':
            # Grid layout (requires frames_count to determine grid size)
            if frames_count is None:
                frames_count = 1
                
            # Calculate grid dimensions
            cols = int((image.width + frame_width - 1) // frame_width)
            rows = int((image.height + frame_height - 1) // frame_height)
            max_possible_frames = cols * rows
            
            actual_frames = min(frames_count, max_possible_frames)
            
            for i in range(actual_frames):
                col = i % cols
                row = i // cols
                
                if row >= rows:
                    break
                    
                left = col * frame_width
                top = row * frame_height
                right = min(left + frame_width, image.width)
                bottom = min(top + frame_height, image.height)
                
                frame = image.crop((left, top, right, bottom))
                if frame.size != (frame_width, frame_height):
                    frame = frame.resize((frame_width, frame_height), Image.Resampling.LANCZOS)
                frames.append(frame)
                
        elif frames_layout == 'auto':
            # Auto-detect based on image dimensions and frame size
            if image.width >= image.height:
                # Wider image - assume horizontal strip
                return self.extract_frames_from_image(image, frame_width, frame_height, 'horizontal', frames_count)
            else:
                # Taller image - assume vertical strip
                return self.extract_frames_from_image(image, frame_width, frame_height, 'vertical', frames_count)
        
        return frames
    
    def convert_image_to_sprite(self, input_path, output_path=None, 
                              sprite_type=2, text_format=0, 
                              frame_width=None, frame_height=None,
                              frames_layout='single', frames_count=None,
                              preserve_transparency=True, frame_groups=None):
        """Convert an image to sprite format with multi-frame support, RGBA handling, and group support."""
        
        if not os.path.exists(input_path):
            raise FileNotFoundError(f"Input file not found: {input_path}")
            
        # Open and process image
        try:
            image = Image.open(input_path)
        except Exception as e:
            raise ValueError(f"Cannot open image: {e}")
            
        # Check if image has transparency
        has_transparency = (image.mode in ('RGBA', 'LA') or 'transparency' in image.info)
        
        # Set output path if not provided
        if output_path is None:
            base_name = os.path.splitext(input_path)[0]
            output_path = f"{base_name}.spr"
            
        # Handle frame dimensions
        if frame_width is None:
            frame_width = image.width
        if frame_height is None:
            frame_height = image.height
            
        # Extract frames from image
        frames = self.extract_frames_from_image(image, frame_width, frame_height, 
                                              frames_layout, frames_count)
        
        if not frames:
            raise ValueError("No frames could be extracted from the image")
            
        print(f"Extracted {len(frames)} frames from image")
        if has_transparency and preserve_transparency:
            print("Detected transparency - using RGBA mode")
        
        # Handle frame groups
        if frame_groups is None:
            # Default: all frames in group 0
            frame_groups = [0] * len(frames)
        elif isinstance(frame_groups, int):
            # Single group number for all frames
            frame_groups = [frame_groups] * len(frames)
        elif isinstance(frame_groups, (list, tuple)):
            # List of group numbers
            if len(frame_groups) < len(frames):
                # Extend with last group number or 0
                last_group = frame_groups[-1] if frame_groups else 0
                frame_groups = list(frame_groups) + [last_group] * (len(frames) - len(frame_groups))
            elif len(frame_groups) > len(frames):
                # Trim to match frame count
                frame_groups = frame_groups[:len(frames)]
        else:
            raise ValueError("frame_groups must be None, int, or list/tuple of ints")
        
        # Validate group numbers
        frame_groups = [max(0, int(g)) for g in frame_groups]  # Ensure non-negative integers
        unique_groups = set(frame_groups)
        
        print(f"Frame groups: {frame_groups}")
        print(f"Unique groups: {sorted(unique_groups)}")
        
        # Create a combined image for palette generation (use all frames)
        combined_width = frame_width * min(len(frames), 10)  # Limit to avoid memory issues
        combined_height = frame_height
        if len(frames) > 10:
            combined_height *= (len(frames) + 9) // 10
            
        # Use appropriate mode for combined image
        combined_mode = 'RGBA' if (has_transparency and preserve_transparency) else 'RGB'
        combined_image = Image.new(combined_mode, (combined_width, combined_height), (0, 0, 0, 0) if combined_mode == 'RGBA' else (0, 0, 0))
        
        # Paste frames into combined image for palette generation
        for i, frame in enumerate(frames[:100]):  # Limit to 100 frames for palette
            # Convert frame to appropriate mode
            if has_transparency and preserve_transparency:
                if frame.mode != 'RGBA':
                    frame = frame.convert('RGBA')
            else:
                if frame.mode != 'RGB':
                    frame = frame.convert('RGB')
                    
            col = i % 10
            row = i // 10
            x = col * frame_width
            y = row * frame_height
            if x + frame_width <= combined_width and y + frame_height <= combined_height:
                combined_image.paste(frame, (x, y))
        
        # Create palette from combined image
        palette, _, has_transparency_final, transparency_index = self.create_palette(combined_image, preserve_transparency)
        
        # Adjust text format for transparency
        if has_transparency_final and preserve_transparency:
            if text_format == 0:  # If normal, switch to index alpha for transparency
                text_format = self.TEXT_INDEXALPHA
        
        # Calculate sprite properties
        bounding_radius = max(frame_width, frame_height) / 2.0
        max_width = frame_width
        max_height = frame_height
        nb_frames = len(frames)
        
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
            
            # Write frames
            for frame_idx, frame in enumerate(frames):
                # Write frame header with group information
                frame_group = frame_groups[frame_idx]
                f.write(struct.pack('<I', frame_group))  # group
                f.write(struct.pack('<i', 0))  # originX
                f.write(struct.pack('<i', 0))  # originY
                f.write(struct.pack('<I', frame_width))  # width
                f.write(struct.pack('<I', frame_height))  # height
                
                # Convert frame to appropriate mode and handle transparency
                if has_transparency_final and preserve_transparency:
                    if frame.mode != 'RGBA':
                        frame = frame.convert('RGBA')
                    
                    # Create indexed version with transparency handling
                    rgb_frame = Image.new('RGB', frame.size)
                    alpha_mask = []
                    
                    for y in range(frame.height):
                        for x in range(frame.width):
                            pixel = frame.getpixel((x, y))
                            if len(pixel) >= 4:  # RGBA
                                r, g, b, a = pixel
                                if a < 128:  # Treat as transparent
                                    rgb_frame.putpixel((x, y), (255, 0, 255))  # Magenta for transparency
                                    alpha_mask.append(transparency_index)
                                else:
                                    rgb_frame.putpixel((x, y), (r, g, b))
                                    # Find closest palette color
                                    closest_idx = 0
                                    min_dist = float('inf')
                                    for pal_idx in range(255):  # Don't use transparency index
                                        pal_r = palette[pal_idx * 3]
                                        pal_g = palette[pal_idx * 3 + 1]
                                        pal_b = palette[pal_idx * 3 + 2]
                                        dist = (r - pal_r) ** 2 + (g - pal_g) ** 2 + (b - pal_b) ** 2
                                        if dist < min_dist:
                                            min_dist = dist
                                            closest_idx = pal_idx
                                    alpha_mask.append(closest_idx)
                            else:  # RGB
                                rgb_frame.putpixel((x, y), pixel[:3])
                                alpha_mask.append(0)  # Default to first palette color
                    
                    # Write pixel data using alpha mask
                    for pixel_idx in alpha_mask:
                        f.write(struct.pack('B', pixel_idx))
                else:
                    # Standard RGB handling
                    if frame.mode != 'RGB':
                        frame = frame.convert('RGB')
                    
                    # Quantize frame using the shared palette
                    quantized_frame = frame.quantize(colors=256, method=Image.Quantize.MEDIANCUT)
                    
                    # Ensure frame is correct size
                    if quantized_frame.size != (frame_width, frame_height):
                        quantized_frame = quantized_frame.resize((frame_width, frame_height), Image.Resampling.LANCZOS)
                    
                    # Write pixel data
                    pixel_data = list(quantized_frame.getdata())
                    for pixel in pixel_data:
                        f.write(struct.pack('B', pixel))
                
        print(f"Sprite saved to: {output_path}")
        print(f"Dimensions: {frame_width}x{frame_height}")
        print(f"Frames: {nb_frames}")
        print(f"Colors: 256")
        if has_transparency_final and preserve_transparency:
            print(f"Transparency: Yes (index {transparency_index})")
            print(f"Text Format: {text_format} (INDEXALPHA)" if text_format == self.TEXT_INDEXALPHA else f"Text Format: {text_format}")
        return output_path

def main():
    parser = argparse.ArgumentParser(description='Convert images to .spr sprite format with multi-frame support')
    parser.add_argument('input', help='Input image file path')
    parser.add_argument('-o', '--output', help='Output sprite file path (optional)')
    parser.add_argument('-w', '--width', type=int, help='Frame width (default: image width)')
    parser.add_argument('--height', type=int, help='Frame height (default: image height)')
    parser.add_argument('-t', '--type', type=int, default=2, choices=[0,1,2,3,4],
                       help='Sprite type: 0=VP_PARALLEL_UPRIGHT, 1=FACING_UPRIGHT, 2=VP_PARALLEL, 3=ORIENTED, 4=VP_PARALLEL_ORIENTED')
    parser.add_argument('-f', '--format', type=int, default=0, choices=[0,1,2,3],
                       help='Text format: 0=NORMAL, 1=ADDITIVE, 2=INDEXALPHA, 3=ALPHTEST')
    
    # Multi-frame options
    parser.add_argument('--frames-layout', choices=['single', 'horizontal', 'vertical', 'grid', 'auto'], 
                       default='single', help='Frame layout: single=one frame, horizontal=frames in row, vertical=frames in column, grid=2D grid, auto=detect automatically')
    parser.add_argument('--frames-count', type=int, help='Number of frames to extract (optional, auto-detected if not specified)')
    
    # Group options
    parser.add_argument('--groups', type=str, help='Frame groups as comma-separated values (e.g., "0,0,1,1,2") or single group number for all frames')
    
    # RGBA/Transparency options
    parser.add_argument('--no-transparency', action='store_true', help='Disable transparency handling (force RGB mode)')
    
    args = parser.parse_args()
    
    # Parse groups
    frame_groups = None
    if args.groups:
        try:
            if ',' in args.groups:
                # Comma-separated group numbers
                frame_groups = [int(x.strip()) for x in args.groups.split(',')]
            else:
                # Single group number for all frames
                frame_groups = int(args.groups)
        except ValueError:
            print("Error: Invalid group format. Use comma-separated integers or single integer.", file=sys.stderr)
            sys.exit(1)
    
    converter = SpriteConverter()
    
    try:
        converter.convert_image_to_sprite(
            input_path=args.input,
            output_path=args.output,
            sprite_type=args.type,
            text_format=args.format,
            frame_width=args.width,
            frame_height=args.height,
            frames_layout=args.frames_layout,
            frames_count=args.frames_count,
            preserve_transparency=not args.no_transparency,
            frame_groups=frame_groups
        )
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)

def interactive_mode():
    """Interactive mode for user input."""
    converter = SpriteConverter()
    
    print("=== Image to Sprite Converter (Multi-Frame Support) ===")
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
    
    # Get frame layout options
    print("\nFrame Layout Options:")
    print("  single     - Single frame (default)")
    print("  horizontal - Frames arranged horizontally (sprite strip)")
    print("  vertical   - Frames arranged vertically")
    print("  grid       - Frames in a 2D grid")
    print("  auto       - Auto-detect based on image dimensions")
    
    frames_layout = input("Frame layout (default: single): ").strip().lower()
    if not frames_layout or frames_layout not in ['single', 'horizontal', 'vertical', 'grid', 'auto']:
        frames_layout = 'single'
    
    frames_count = None
    if frames_layout != 'single':
        frames_count_input = input("Number of frames (optional, auto-detect if blank): ").strip()
        if frames_count_input:
            try:
                frames_count = int(frames_count_input)
            except ValueError:
                print("Invalid frame count, using auto-detection")
    
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
            frame_height=frame_height,
            frames_layout=frames_layout,
            frames_count=frames_count
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