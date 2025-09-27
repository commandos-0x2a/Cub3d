#!/usr/bin/env python3
"""
Sprite Image Extractor
Extracts images from .spr sprite files and saves them as standard image formats.
"""

import struct
import sys
import os
import argparse
from PIL import Image

class SpriteExtractor:
    def __init__(self):
        self.SPRITE_ID = b'IDSP'  # Format ID
        self.VERSION = 2          # HL1 sprite version
        
    def read_sprite_header(self, f):
        """Read sprite header from file."""
        header = {}
        
        # Read sprite header fields
        header['id'] = f.read(4)
        header['version'] = struct.unpack('<I', f.read(4))[0]
        header['type'] = struct.unpack('<I', f.read(4))[0]
        header['text_format'] = struct.unpack('<I', f.read(4))[0]
        header['bounding_radius'] = struct.unpack('<f', f.read(4))[0]
        header['max_width'] = struct.unpack('<I', f.read(4))[0]
        header['max_height'] = struct.unpack('<I', f.read(4))[0]
        header['nb_frames'] = struct.unpack('<I', f.read(4))[0]
        header['beam_length'] = struct.unpack('<f', f.read(4))[0]
        header['synch_type'] = struct.unpack('<I', f.read(4))[0]
        header['palette_colors'] = struct.unpack('<H', f.read(2))[0]
        
        return header
    
    def validate_sprite_header(self, header):
        """Validate sprite header."""
        if header['id'] != self.SPRITE_ID:
            raise ValueError(f"Invalid sprite ID: {header['id']}")
        if header['version'] != self.VERSION:
            raise ValueError(f"Unsupported sprite version: {header['version']}")
        if header['palette_colors'] != 256:
            raise ValueError(f"Expected 256 palette colors, got {header['palette_colors']}")
        return True
    
    def read_palette(self, f):
        """Read 256-color palette from sprite file."""
        palette = []
        for i in range(256):
            r, g, b = struct.unpack('BBB', f.read(3))
            palette.append((r, g, b))
        return palette
    
    def read_frame_header(self, f):
        """Read frame header from sprite file."""
        frame_header = {}
        frame_header['group'] = struct.unpack('<I', f.read(4))[0]
        frame_header['origin_x'] = struct.unpack('<i', f.read(4))[0]
        frame_header['origin_y'] = struct.unpack('<i', f.read(4))[0]
        frame_header['width'] = struct.unpack('<I', f.read(4))[0]
        frame_header['height'] = struct.unpack('<I', f.read(4))[0]
        return frame_header
    
    def read_frame_pixels(self, f, width, height):
        """Read pixel data for a frame."""
        pixel_count = width * height
        pixel_data = []
        for i in range(pixel_count):
            pixel_index = struct.unpack('B', f.read(1))[0]
            pixel_data.append(pixel_index)
        return pixel_data
    
    def create_image_from_frame(self, frame_pixels, width, height, palette, text_format=0, transparency_index=255):
        """Create PIL Image from frame pixel data and palette with RGBA support."""
        # Determine if transparency is used based on text format
        has_transparency = (text_format == 2)  # INDEXALPHA format
        
        # Create appropriate image mode
        if has_transparency:
            image = Image.new('RGBA', (width, height))
            pixels = []
            
            for pixel_index in frame_pixels:
                if pixel_index == transparency_index:
                    # Transparent pixel
                    pixels.append((0, 0, 0, 0))
                elif pixel_index < len(palette):
                    r, g, b = palette[pixel_index]
                    pixels.append((r, g, b, 255))  # Fully opaque
                else:
                    pixels.append((0, 0, 0, 255))  # Default to black opaque for invalid indices
        else:
            # Standard RGB mode
            image = Image.new('RGB', (width, height))
            pixels = []
            
            for pixel_index in frame_pixels:
                if pixel_index < len(palette):
                    pixels.append(palette[pixel_index])
                else:
                    pixels.append((0, 0, 0))  # Default to black for invalid indices
        
        image.putdata(pixels)
        return image
    
    def extract_sprite_images(self, sprite_path, output_dir=None, output_format='PNG', 
                            frame_indices=None, prefix=None):
        """Extract images from sprite file."""
        
        if not os.path.exists(sprite_path):
            raise FileNotFoundError(f"Sprite file not found: {sprite_path}")
        
        # Set output directory
        if output_dir is None:
            output_dir = os.path.dirname(sprite_path) or '.'
        
        # Create output directory if it doesn't exist
        os.makedirs(output_dir, exist_ok=True)
        
        # Set prefix for output files
        if prefix is None:
            prefix = os.path.splitext(os.path.basename(sprite_path))[0]
        
        extracted_files = []
        
        with open(sprite_path, 'rb') as f:
            # Read and validate header
            header = self.read_sprite_header(f)
            self.validate_sprite_header(header)
            
            print(f"Sprite Info:")
            print(f"  Dimensions: {header['max_width']}x{header['max_height']}")
            print(f"  Frames: {header['nb_frames']}")
            print(f"  Type: {header['type']}")
            print(f"  Text Format: {header['text_format']}")
            
            # Check for transparency
            has_transparency = (header['text_format'] == 2)  # INDEXALPHA
            transparency_index = 255 if has_transparency else None
            if has_transparency:
                print(f"  Transparency: Yes (index {transparency_index})")
            
            # Read palette
            palette = self.read_palette(f)
            print(f"  Palette Colors: {len(palette)}")
            
            # Determine which frames to extract
            if frame_indices is None:
                frame_indices = list(range(header['nb_frames']))
            else:
                # Validate frame indices
                frame_indices = [i for i in frame_indices if 0 <= i < header['nb_frames']]
            
            # Extract frames
            group_info = {}  # Track group information
            for frame_idx in range(header['nb_frames']):
                # Read frame header
                frame_header = self.read_frame_header(f)
                
                # Track group information
                group_id = frame_header['group']
                if group_id not in group_info:
                    group_info[group_id] = []
                group_info[group_id].append(frame_idx)
                
                # Read frame pixels
                frame_pixels = self.read_frame_pixels(f, frame_header['width'], frame_header['height'])
                
                # Skip frames not in our extraction list
                if frame_idx not in frame_indices:
                    continue
                
                # Create image from frame data
                image = self.create_image_from_frame(frame_pixels, 
                                                   frame_header['width'], 
                                                   frame_header['height'], 
                                                   palette,
                                                   header['text_format'],
                                                   transparency_index)
                
                # Generate output filename
                if header['nb_frames'] == 1:
                    output_filename = f"{prefix}.{output_format.lower()}"
                else:
                    output_filename = f"{prefix}_frame_{frame_idx:03d}.{output_format.lower()}"
                
                output_path = os.path.join(output_dir, output_filename)
                
                # Save image
                image.save(output_path, output_format)
                extracted_files.append(output_path)
                
                print(f"  Extracted frame {frame_idx} (group {group_id}): {output_path} ({frame_header['width']}x{frame_header['height']})")
            
            # Display group summary
            if len(group_info) > 1:
                print(f"\nGroup Summary:")
                for group_id in sorted(group_info.keys()):
                    frames_in_group = group_info[group_id]
                    print(f"  Group {group_id}: {len(frames_in_group)} frames {frames_in_group}")
        
        return extracted_files
    
    def get_frames_by_groups(self, sprite_path, group_ids):
        """Get frame indices that belong to specified groups."""
        if not os.path.exists(sprite_path):
            raise FileNotFoundError(f"Sprite file not found: {sprite_path}")
        
        frame_indices = []
        
        with open(sprite_path, 'rb') as f:
            # Read and validate header
            header = self.read_sprite_header(f)
            self.validate_sprite_header(header)
            
            # Skip palette
            self.read_palette(f)
            
            # Read frame headers to get group information
            for frame_idx in range(header['nb_frames']):
                frame_header = self.read_frame_header(f)
                
                if frame_header['group'] in group_ids:
                    frame_indices.append(frame_idx)
                
                # Skip pixel data
                pixel_count = frame_header['width'] * frame_header['height']
                f.seek(pixel_count, 1)  # Seek forward past pixel data
        
        return frame_indices
    
    def extract_sprite_sheet(self, sprite_path, output_path=None, layout='horizontal'):
        """Extract all frames as a single sprite sheet."""
        
        if not os.path.exists(sprite_path):
            raise FileNotFoundError(f"Sprite file not found: {sprite_path}")
        
        # Set output path
        if output_path is None:
            base_name = os.path.splitext(sprite_path)[0]
            output_path = f"{base_name}_sheet.png"
        
        with open(sprite_path, 'rb') as f:
            # Read and validate header
            header = self.read_sprite_header(f)
            self.validate_sprite_header(header)
            
            # Read palette
            palette = self.read_palette(f)
            
            # Check for transparency
            has_transparency = (header['text_format'] == 2)  # INDEXALPHA
            transparency_index = 255 if has_transparency else None
            
            # Read all frames
            frames = []
            frame_groups = []
            for frame_idx in range(header['nb_frames']):
                frame_header = self.read_frame_header(f)
                frame_pixels = self.read_frame_pixels(f, frame_header['width'], frame_header['height'])
                
                image = self.create_image_from_frame(frame_pixels, 
                                                   frame_header['width'], 
                                                   frame_header['height'], 
                                                   palette,
                                                   header['text_format'],
                                                   transparency_index)
                frames.append(image)
                frame_groups.append(frame_header['group'])
            
            if not frames:
                raise ValueError("No frames found in sprite")
            
            # Display group information
            unique_groups = set(frame_groups)
            if len(unique_groups) > 1:
                group_info = {}
                for i, group_id in enumerate(frame_groups):
                    if group_id not in group_info:
                        group_info[group_id] = []
                    group_info[group_id].append(i)
                
                print(f"Groups in sprite:")
                for group_id in sorted(group_info.keys()):
                    frames_in_group = group_info[group_id]
                    print(f"  Group {group_id}: {len(frames_in_group)} frames {frames_in_group}")
            
            # Create sprite sheet
            frame_width = frames[0].width
            frame_height = frames[0].height
            
            # Determine sheet mode based on first frame
            sheet_mode = frames[0].mode
            background_color = (0, 0, 0, 0) if sheet_mode == 'RGBA' else (0, 0, 0)
            
            if layout == 'horizontal':
                sheet_width = frame_width * len(frames)
                sheet_height = frame_height
                sheet = Image.new(sheet_mode, (sheet_width, sheet_height), background_color)
                
                for i, frame in enumerate(frames):
                    sheet.paste(frame, (i * frame_width, 0))
                    
            elif layout == 'vertical':
                sheet_width = frame_width
                sheet_height = frame_height * len(frames)
                sheet = Image.new(sheet_mode, (sheet_width, sheet_height), background_color)
                
                for i, frame in enumerate(frames):
                    sheet.paste(frame, (0, i * frame_height))
                    
            elif layout == 'grid':
                # Calculate grid dimensions
                import math
                cols = math.ceil(math.sqrt(len(frames)))
                rows = math.ceil(len(frames) / cols)
                
                sheet_width = frame_width * cols
                sheet_height = frame_height * rows
                sheet = Image.new(sheet_mode, (sheet_width, sheet_height), background_color)
                
                for i, frame in enumerate(frames):
                    col = i % cols
                    row = i // cols
                    sheet.paste(frame, (col * frame_width, row * frame_height))
            
            else:
                raise ValueError(f"Unknown layout: {layout}")
            
            # Save sprite sheet
            sheet.save(output_path)
            print(f"Sprite sheet saved: {output_path} ({sheet.width}x{sheet.height})")
            print(f"Layout: {layout}, Frames: {len(frames)}")
            
            return output_path

def main():
    parser = argparse.ArgumentParser(description='Extract images from .spr sprite files')
    parser.add_argument('input', help='Input sprite file path')
    parser.add_argument('-o', '--output-dir', help='Output directory (default: same as input)')
    parser.add_argument('-f', '--format', choices=['PNG', 'BMP', 'JPEG', 'TIFF'], 
                       default='PNG', help='Output image format')
    parser.add_argument('-p', '--prefix', help='Output filename prefix (default: sprite filename)')
    parser.add_argument('--frames', type=str, help='Comma-separated frame indices to extract (e.g., "0,1,3")')
    parser.add_argument('--groups', type=str, help='Comma-separated group IDs to extract (e.g., "0,2") - extracts all frames from specified groups')
    parser.add_argument('--sheet', action='store_true', help='Extract as sprite sheet instead of individual frames')
    parser.add_argument('--sheet-layout', choices=['horizontal', 'vertical', 'grid'], 
                       default='horizontal', help='Sprite sheet layout')
    parser.add_argument('--sheet-output', help='Output path for sprite sheet')
    
    args = parser.parse_args()
    
    extractor = SpriteExtractor()
    
    try:
        if args.sheet:
            # Extract as sprite sheet
            output_path = extractor.extract_sprite_sheet(
                args.input, 
                args.sheet_output, 
                args.sheet_layout
            )
            print(f"Success! Sprite sheet saved to: {output_path}")
        else:
            # Extract individual frames
            frame_indices = None
            
            if args.frames and args.groups:
                print("Error: Cannot specify both --frames and --groups options simultaneously.")
                sys.exit(1)
            
            if args.frames:
                try:
                    frame_indices = [int(x.strip()) for x in args.frames.split(',')]
                except ValueError:
                    print("Error: Invalid frame indices format. Use comma-separated integers.")
                    sys.exit(1)
            elif args.groups:
                try:
                    group_ids = [int(x.strip()) for x in args.groups.split(',')]
                    frame_indices = extractor.get_frames_by_groups(args.input, group_ids)
                    print(f"Extracting frames from groups {group_ids}: {frame_indices}")
                except ValueError:
                    print("Error: Invalid group format. Use comma-separated integers.")
                    sys.exit(1)
            
            extracted_files = extractor.extract_sprite_images(
                args.input,
                args.output_dir,
                args.format,
                frame_indices,
                args.prefix
            )
            
            print(f"Success! Extracted {len(extracted_files)} images")
            
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)

def interactive_mode():
    """Interactive mode for sprite extraction."""
    extractor = SpriteExtractor()
    
    print("=== Sprite Image Extractor ===")
    print()
    
    # Get input file
    while True:
        sprite_path = input("Enter sprite file path (.spr): ").strip()
        if os.path.exists(sprite_path):
            break
        print("File not found. Please try again.")
    
    # Get extraction mode
    print("\nExtraction Options:")
    print("  1. Individual frames")
    print("  2. Sprite sheet")
    
    while True:
        choice = input("Choose extraction mode (1 or 2): ").strip()
        if choice in ['1', '2']:
            break
        print("Please enter 1 or 2.")
    
    try:
        if choice == '1':
            # Individual frames mode
            output_dir = input("Output directory (default: same as sprite): ").strip()
            if not output_dir:
                output_dir = None
            
            format_choice = input("Output format (PNG, BMP, JPEG, TIFF) [PNG]: ").strip().upper()
            if not format_choice:
                format_choice = 'PNG'
            
            frames_input = input("Frame indices to extract (comma-separated, blank for all): ").strip()
            frame_indices = None
            if frames_input:
                try:
                    frame_indices = [int(x.strip()) for x in frames_input.split(',')]
                except ValueError:
                    print("Invalid frame indices, extracting all frames.")
                    frame_indices = None
            
            extracted_files = extractor.extract_sprite_images(
                sprite_path, output_dir, format_choice, frame_indices
            )
            print(f"\nSuccess! Extracted {len(extracted_files)} images")
            
        else:
            # Sprite sheet mode
            output_path = input("Output sprite sheet path (default: auto): ").strip()
            if not output_path:
                output_path = None
            
            layout = input("Layout (horizontal, vertical, grid) [horizontal]: ").strip().lower()
            if layout not in ['horizontal', 'vertical', 'grid']:
                layout = 'horizontal'
            
            output_path = extractor.extract_sprite_sheet(sprite_path, output_path, layout)
            print(f"\nSuccess! Sprite sheet saved to: {output_path}")
            
    except Exception as e:
        print(f"\nError: {e}")

if __name__ == "__main__":
    if len(sys.argv) == 1:
        # No arguments provided, run interactive mode
        interactive_mode()
    else:
        # Command line arguments provided
        main()