#!/usr/bin/env python3
"""
Simple sprite file reader/validator for testing the converter
"""

import struct
import sys

def read_sprite_header(filename):
    """Read and display sprite header information."""
    try:
        with open(filename, 'rb') as f:
            # Read header
            sprite_id = f.read(4)
            version = struct.unpack('<I', f.read(4))[0]
            sprite_type = struct.unpack('<I', f.read(4))[0]
            text_format = struct.unpack('<I', f.read(4))[0]
            bounding_radius = struct.unpack('<f', f.read(4))[0]
            max_width = struct.unpack('<I', f.read(4))[0]
            max_height = struct.unpack('<I', f.read(4))[0]
            nb_frames = struct.unpack('<I', f.read(4))[0]
            beam_length = struct.unpack('<f', f.read(4))[0]
            synch_type = struct.unpack('<I', f.read(4))[0]
            palette_colors = struct.unpack('<H', f.read(2))[0]
            
            print(f"Sprite File: {filename}")
            print(f"ID: {sprite_id}")
            print(f"Version: {version}")
            print(f"Type: {sprite_type}")
            print(f"Text Format: {text_format}")
            print(f"Bounding Radius: {bounding_radius}")
            print(f"Max Width: {max_width}")
            print(f"Max Height: {max_height}")
            print(f"Number of Frames: {nb_frames}")
            print(f"Beam Length: {beam_length}")
            print(f"Sync Type: {synch_type}")
            print(f"Palette Colors: {palette_colors}")
            
            # Read palette (256 colors * 3 bytes)
            palette_size = 256 * 3
            palette_data = f.read(palette_size)
            print(f"Palette Size: {len(palette_data)} bytes")
            
            # Read frame header
            group = struct.unpack('<I', f.read(4))[0]
            origin_x = struct.unpack('<i', f.read(4))[0]
            origin_y = struct.unpack('<i', f.read(4))[0]
            frame_width = struct.unpack('<I', f.read(4))[0]
            frame_height = struct.unpack('<I', f.read(4))[0]
            
            print(f"Frame Group: {group}")
            print(f"Frame Origin: ({origin_x}, {origin_y})")
            print(f"Frame Size: {frame_width}x{frame_height}")
            
            # Check pixel data size
            expected_pixel_data = frame_width * frame_height
            remaining_data = len(f.read())
            print(f"Expected pixel data: {expected_pixel_data} bytes")
            print(f"Actual pixel data: {remaining_data} bytes")
            
            if expected_pixel_data == remaining_data:
                print("✓ Sprite format validation PASSED")
            else:
                print("✗ Sprite format validation FAILED")
                
    except Exception as e:
        print(f"Error reading sprite: {e}")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python sprite_reader.py <sprite_file.spr>")
        sys.exit(1)
        
    read_sprite_header(sys.argv[1])