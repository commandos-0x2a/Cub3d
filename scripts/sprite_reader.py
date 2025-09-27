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
            if text_format == 2:
                print("  -> INDEXALPHA (Transparency supported)")
            elif text_format == 1:
                print("  -> ADDITIVE")
            elif text_format == 3:
                print("  -> ALPHTEST")
            else:
                print("  -> NORMAL")
            print(f"Bounding Radius: {bounding_radius}")
            print(f"Max Width: {max_width}")
            print(f"Max Height: {max_height}")
            print(f"Number of Frames: {nb_frames}")
            print(f"Beam Length: {beam_length}")
            print(f"Sync Type: {synch_type}")
            print(f"Palette Colors: {palette_colors}")
            
            # Transparency analysis
            has_transparency = (text_format == 2)
            if has_transparency:
                print(f"Transparency: Yes (likely index 255)")
            else:
                print(f"Transparency: No")
            
            # Read palette (256 colors * 3 bytes)
            palette_size = 256 * 3
            palette_data = f.read(palette_size)
            print(f"Palette Size: {len(palette_data)} bytes")
            
            # Check for transparency color (often magenta for sprites)
            if has_transparency and len(palette_data) >= 768:
                transparency_r = palette_data[255 * 3]
                transparency_g = palette_data[255 * 3 + 1] 
                transparency_b = palette_data[255 * 3 + 2]
                print(f"Transparency Color (index 255): RGB({transparency_r}, {transparency_g}, {transparency_b})")
                if transparency_r == 255 and transparency_g == 0 and transparency_b == 255:
                    print("  -> Standard magenta transparency color detected")
            
            # Read frame header
            group = struct.unpack('<I', f.read(4))[0]
            origin_x = struct.unpack('<i', f.read(4))[0]
            origin_y = struct.unpack('<i', f.read(4))[0]
            frame_width = struct.unpack('<I', f.read(4))[0]
            frame_height = struct.unpack('<I', f.read(4))[0]
            
            print(f"First Frame Group: {group}")
            print(f"Frame Origin: ({origin_x}, {origin_y})")
            print(f"Frame Size: {frame_width}x{frame_height}")
            
            # If multiple frames, analyze all groups
            if nb_frames > 1:
                print(f"\nAnalyzing all {nb_frames} frames...")
                
                # Calculate header size: sprite header is 42 bytes (includes 2-byte palette count)
                header_size = 42
                palette_bytes = palette_size
                
                # Reset to start of frame data (after header and palette)
                f.seek(header_size + palette_bytes)
                
                groups = []
                frame_sizes = []
                for frame_idx in range(nb_frames):
                    try:
                        frame_group = struct.unpack('<I', f.read(4))[0]
                        frame_origin_x = struct.unpack('<i', f.read(4))[0]
                        frame_origin_y = struct.unpack('<i', f.read(4))[0]
                        frame_w = struct.unpack('<I', f.read(4))[0]
                        frame_h = struct.unpack('<I', f.read(4))[0]
                        
                        groups.append(frame_group)
                        frame_sizes.append((frame_w, frame_h))
                        
                        # Skip pixel data for this frame
                        pixel_data_size = frame_w * frame_h
                        f.seek(pixel_data_size, 1)  # Skip pixel data
                    except struct.error as e:
                        print(f"Error reading frame {frame_idx}: {e}")
                        break
                
                # Analyze group distribution
                from collections import Counter
                group_counts = Counter(groups)
                unique_groups = sorted(group_counts.keys())
                
                print(f"Group Analysis:")
                print(f"  Total Groups: {len(unique_groups)}")
                print(f"  Group Distribution:")
                
                for group_id in unique_groups:
                    frame_indices = [i for i, g in enumerate(groups) if g == group_id]
                    print(f"    Group {group_id}: {group_counts[group_id]} frames {frame_indices}")
                
                # Check pixel data size for all frames
                total_expected_pixels = sum(w * h for w, h in frame_sizes)
                current_pos = f.tell()
                f.seek(0, 2)  # Seek to end
                file_size = f.tell()
                frame_headers_size = 20 * len(groups)  # 20 bytes per frame header
                actual_pixel_data = file_size - (header_size + palette_bytes + frame_headers_size)
                
                print(f"\nData Validation:")
                print(f"Expected pixel data: {total_expected_pixels} bytes")
                print(f"Actual pixel data: {actual_pixel_data} bytes")
                
                if total_expected_pixels == actual_pixel_data:
                    print("✓ Sprite format validation PASSED")
                else:
                    print("✗ Sprite format validation FAILED")
            else:
                # Single frame - check pixel data size normally
                expected_pixel_data = frame_width * frame_height * nb_frames
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