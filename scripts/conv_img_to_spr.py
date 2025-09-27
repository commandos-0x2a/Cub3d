
#!/usr/bin/env python3
"""
Image to Sprite Converter - GUI Version
Converts any image format to .spr sprite format based on Half-Life 1 sprite specification.
"""

import struct
import sys
import os
from PIL import Image, ImageTk
import argparse
import tkinter as tk
from tkinter import ttk, filedialog, messagebox, scrolledtext
import threading

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
        
        # Frame types
        self.FRAME_TYPE_SINGLE = 0
        self.FRAME_TYPE_GROUP = 2
        
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

    def parse_group_config(self, group_config, total_frames):
        """Parse group configuration string into frame groups with intervals."""
        if not group_config:
            return None
            
        groups = {}
        
        # Split by semicolon for different groups
        group_parts = group_config.split(';')
        
        for group_part in group_parts:
            group_part = group_part.strip()
            if not group_part:
                continue
                
            # Parse group format: "group_id:frame_start-frame_end:interval1,interval2,..."
            # or "group_id:frame_indices:intervals"
            parts = group_part.split(':')
            if len(parts) < 2:
                continue
                
            try:
                group_id = int(parts[0])
                
                # Parse frame specification
                if '-' in parts[1]:
                    # Range format: "0-3" means frames 0,1,2,3
                    start, end = map(int, parts[1].split('-'))
                    frame_indices = list(range(start, end + 1))
                else:
                    # Comma-separated indices: "0,2,4"
                    frame_indices = [int(x.strip()) for x in parts[1].split(',')]
                
                # Parse intervals
                intervals = []
                if len(parts) >= 3 and parts[2]:
                    intervals = [float(x.strip()) for x in parts[2].split(',')]
                else:
                    # Default intervals (0.1 seconds per frame)
                    intervals = [0.1] * len(frame_indices)
                
                # Extend or trim intervals to match frame count
                if len(intervals) < len(frame_indices):
                    intervals.extend([intervals[-1] if intervals else 0.1] * (len(frame_indices) - len(intervals)))
                elif len(intervals) > len(frame_indices):
                    intervals = intervals[:len(frame_indices)]
                
                groups[group_id] = {
                    'frames': frame_indices,
                    'intervals': intervals
                }
                
            except (ValueError, IndexError) as e:
                print(f"Warning: Invalid group specification '{group_part}': {e}")
                continue
        
        return groups if groups else None

    def write_frame_pixels(self, f, frame, palette, has_transparency_final, 
                          preserve_transparency, transparency_index):
        """Write frame pixel data to file."""
        if has_transparency_final and preserve_transparency:
            if frame.mode != 'RGBA':
                frame = frame.convert('RGBA')
            
            # Create indexed version with transparency handling
            for y in range(frame.height):
                for x in range(frame.width):
                    pixel = frame.getpixel((x, y))
                    if len(pixel) >= 4:  # RGBA
                        r, g, b, a = pixel
                        if a < 128:  # Treat as transparent
                            f.write(struct.pack('B', transparency_index))
                        else:
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
                            f.write(struct.pack('B', closest_idx))
                    else:  # RGB
                        f.write(struct.pack('B', 0))  # Default to first palette color
        else:
            # Standard RGB handling
            if frame.mode != 'RGB':
                frame = frame.convert('RGB')
            
            # Quantize frame using the shared palette
            quantized_frame = frame.quantize(colors=256, method=Image.Quantize.MEDIANCUT)
            
            # Write pixel data
            pixel_data = list(quantized_frame.getdata())
            for pixel in pixel_data:
                f.write(struct.pack('B', pixel))
    
    def convert_image_to_sprite(self, input_path, output_path=None, 
                              sprite_type=2, text_format=0, 
                              frame_width=None, frame_height=None,
                              frames_layout='single', frames_count=None,
                              preserve_transparency=True, group_config=None):
        """Convert an image to sprite format with group support and intervals."""
        
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
        
        # Parse group configuration
        groups = self.parse_group_config(group_config, len(frames))
        
        # Create frame structure
        frame_structure = []
        used_frames = set()
        
        if groups:
            # Process groups
            for group_id, group_data in sorted(groups.items()):
                frame_indices = group_data['frames']
                intervals = group_data['intervals']
                
                # Validate frame indices
                valid_indices = [i for i in frame_indices if 0 <= i < len(frames)]
                if not valid_indices:
                    print(f"Warning: Group {group_id} has no valid frame indices")
                    continue
                
                # Add group frame
                frame_structure.append({
                    'type': self.FRAME_TYPE_GROUP,
                    'group_id': group_id,
                    'frame_indices': valid_indices,
                    'intervals': intervals[:len(valid_indices)]
                })
                
                used_frames.update(valid_indices)
                print(f"Group {group_id}: frames {valid_indices}, intervals {intervals[:len(valid_indices)]}")
            
            # Add remaining frames as single frames
            for i in range(len(frames)):
                if i not in used_frames:
                    frame_structure.append({
                        'type': self.FRAME_TYPE_SINGLE,
                        'frame_index': i
                    })
                    print(f"Single frame: {i}")
        else:
            # No groups - all frames are single frames
            for i in range(len(frames)):
                frame_structure.append({
                    'type': self.FRAME_TYPE_SINGLE,
                    'frame_index': i
                })
        
        # Create combined image for palette generation
        combined_width = frame_width * min(len(frames), 10)
        combined_height = frame_height
        if len(frames) > 10:
            combined_height *= (len(frames) + 9) // 10
            
        combined_mode = 'RGBA' if (has_transparency and preserve_transparency) else 'RGB'
        combined_image = Image.new(combined_mode, (combined_width, combined_height), 
                                 (0, 0, 0, 0) if combined_mode == 'RGBA' else (0, 0, 0))
        
        # Paste frames into combined image for palette generation
        for i, frame in enumerate(frames[:100]):
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
        if has_transparency_final and preserve_transparency and text_format == 0:
            text_format = self.TEXT_INDEXALPHA
        
        # Calculate sprite properties
        bounding_radius = max(frame_width, frame_height) / 2.0
        max_width = frame_width
        max_height = frame_height
        nb_sprite_frames = len(frame_structure)  # Number of sprite frames (groups + singles)
        
        # Write sprite file
        with open(output_path, 'wb') as f:
            # Write sprite header (42 bytes)
            f.write(self.SPRITE_ID)  # 4 bytes
            f.write(struct.pack('<I', self.VERSION))  # 4 bytes
            f.write(struct.pack('<I', sprite_type))  # 4 bytes
            f.write(struct.pack('<I', text_format))  # 4 bytes
            f.write(struct.pack('<f', bounding_radius))  # 4 bytes
            f.write(struct.pack('<I', max_width))  # 4 bytes
            f.write(struct.pack('<I', max_height))  # 4 bytes
            f.write(struct.pack('<I', nb_sprite_frames))  # 4 bytes
            f.write(struct.pack('<f', 0.0))  # beam_length - 4 bytes
            f.write(struct.pack('<I', 0))  # synch_type - 4 bytes
            f.write(struct.pack('<H', 256))  # palette_color_count - 2 bytes
            
            # Write palette (768 bytes)
            for i in range(0, 768, 3):
                r = palette[i] if i < len(palette) else 0
                g = palette[i+1] if i+1 < len(palette) else 0
                b = palette[i+2] if i+2 < len(palette) else 0
                f.write(struct.pack('BBB', r, g, b))
            
            # Write sprite frames
            for sprite_frame in frame_structure:
                # Write frame type
                f.write(struct.pack('<I', sprite_frame['type']))
                
                if sprite_frame['type'] == self.FRAME_TYPE_GROUP:
                    # Write group frame
                    frame_indices = sprite_frame['frame_indices']
                    intervals = sprite_frame['intervals']
                    
                    # Write group header
                    f.write(struct.pack('<I', len(frame_indices)))  # nb_frame
                    
                    # Write intervals
                    for interval in intervals:
                        f.write(struct.pack('<f', interval))
                    
                    # Write each frame in group
                    for frame_idx in frame_indices:
                        frame = frames[frame_idx]
                        
                        # Write frame header
                        f.write(struct.pack('<i', 0))  # originX
                        f.write(struct.pack('<i', 0))  # originY
                        f.write(struct.pack('<I', frame_width))  # width
                        f.write(struct.pack('<I', frame_height))  # height
                        
                        # Write pixel data
                        self.write_frame_pixels(f, frame, palette, has_transparency_final, 
                                              preserve_transparency, transparency_index)
                        
                else:
                    # Write single frame
                    frame_idx = sprite_frame['frame_index']
                    frame = frames[frame_idx]
                    
                    # Write frame header
                    f.write(struct.pack('<i', 0))  # originX
                    f.write(struct.pack('<i', 0))  # originY
                    f.write(struct.pack('<I', frame_width))  # width
                    f.write(struct.pack('<I', frame_height))  # height
                    
                    # Write pixel data
                    self.write_frame_pixels(f, frame, palette, has_transparency_final, 
                                          preserve_transparency, transparency_index)
                
        print(f"Sprite saved to: {output_path}")
        print(f"Dimensions: {frame_width}x{frame_height}")
        print(f"Total image frames: {len(frames)}")
        print(f"Sprite frames: {nb_sprite_frames}")
        print(f"Colors: 256")
        if has_transparency_final and preserve_transparency:
            print(f"Transparency: Yes (index {transparency_index})")
        return output_path

class SpriteConverterGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Sprite Converter - Image to .spr")
        self.root.geometry("800x900")
        self.root.resizable(True, True)
        
        self.converter = SpriteConverter()
        self.current_image = None
        self.preview_image = None
        
        self.setup_ui()
        
    def setup_ui(self):
        # Create main frame with scrollbar
        main_frame = ttk.Frame(self.root)
        main_frame.pack(fill=tk.BOTH, expand=True, padx=10, pady=10)
        
        # File Selection Section
        file_frame = ttk.LabelFrame(main_frame, text="File Selection", padding=10)
        file_frame.pack(fill=tk.X, pady=(0, 10))
        
        self.file_path = tk.StringVar()
        ttk.Label(file_frame, text="Input Image:").pack(anchor=tk.W)
        
        file_select_frame = ttk.Frame(file_frame)
        file_select_frame.pack(fill=tk.X, pady=(5, 0))
        
        self.file_entry = ttk.Entry(file_select_frame, textvariable=self.file_path, state="readonly")
        self.file_entry.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=(0, 5))
        
        ttk.Button(file_select_frame, text="Browse", command=self.browse_file).pack(side=tk.RIGHT)
        
        # Try to enable drag and drop if supported
        try:
            self.root.drop_target_register('DND_Files')
            self.root.dnd_bind('<<Drop>>', self.on_drop)
            ttk.Button(file_select_frame, text="Drag & Drop Info", command=self.show_drag_drop_info).pack(side=tk.RIGHT, padx=(0, 5))
        except:
            # Drag and drop not supported on this system
            pass
        
        # Output file
        self.output_path = tk.StringVar()
        ttk.Label(file_frame, text="Output File (optional):").pack(anchor=tk.W, pady=(10, 0))
        
        output_frame = ttk.Frame(file_frame)
        output_frame.pack(fill=tk.X, pady=(5, 0))
        
        self.output_entry = ttk.Entry(output_frame, textvariable=self.output_path)
        self.output_entry.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=(0, 5))
        
        ttk.Button(output_frame, text="Browse", command=self.browse_output).pack(side=tk.RIGHT)
        
        # Image Preview Section
        preview_frame = ttk.LabelFrame(main_frame, text="Image Preview", padding=10)
        preview_frame.pack(fill=tk.X, pady=(0, 10))
        
        self.preview_label = ttk.Label(preview_frame, text="No image loaded", anchor=tk.CENTER)
        self.preview_label.pack(fill=tk.BOTH, expand=True)
        
        # Frame Configuration Section
        frame_config_frame = ttk.LabelFrame(main_frame, text="Frame Configuration", padding=10)
        frame_config_frame.pack(fill=tk.X, pady=(0, 10))
        
        # Frame dimensions
        dims_frame = ttk.Frame(frame_config_frame)
        dims_frame.pack(fill=tk.X, pady=(0, 10))
        
        ttk.Label(dims_frame, text="Frame Width:").pack(side=tk.LEFT)
        self.frame_width = tk.StringVar()
        width_entry = ttk.Entry(dims_frame, textvariable=self.frame_width, width=10)
        width_entry.pack(side=tk.LEFT, padx=(5, 20))
        
        ttk.Label(dims_frame, text="Frame Height:").pack(side=tk.LEFT)
        self.frame_height = tk.StringVar()
        height_entry = ttk.Entry(dims_frame, textvariable=self.frame_height, width=10)
        height_entry.pack(side=tk.LEFT, padx=(5, 0))
        
        ttk.Button(dims_frame, text="Auto", command=self.auto_dimensions).pack(side=tk.RIGHT)
        
        # Frame layout
        layout_frame = ttk.Frame(frame_config_frame)
        layout_frame.pack(fill=tk.X, pady=(0, 10))
        
        ttk.Label(layout_frame, text="Frame Layout:").pack(side=tk.LEFT)
        self.frame_layout = tk.StringVar(value="single")
        layout_combo = ttk.Combobox(layout_frame, textvariable=self.frame_layout, 
                                   values=["single", "horizontal", "vertical", "grid", "auto"],
                                   state="readonly", width=15)
        layout_combo.pack(side=tk.LEFT, padx=(5, 20))
        
        ttk.Label(layout_frame, text="Frame Count:").pack(side=tk.LEFT)
        self.frame_count = tk.StringVar()
        count_entry = ttk.Entry(layout_frame, textvariable=self.frame_count, width=10)
        count_entry.pack(side=tk.LEFT, padx=(5, 0))
        
        # Group Configuration Section
        group_frame = ttk.LabelFrame(main_frame, text="Group Configuration (Advanced)", padding=10)
        group_frame.pack(fill=tk.X, pady=(0, 10))
        
        ttk.Label(group_frame, text="Groups & Intervals:").pack(anchor=tk.W)
        ttk.Label(group_frame, text='Format: "group_id:frame_range:intervals;..." (e.g., "0:0-3:0.1,0.1,0.1,0.1;1:4-7:0.05")', 
                 font=("TkDefaultFont", 8)).pack(anchor=tk.W, pady=(0, 5))
        
        self.groups_config = tk.StringVar()
        groups_entry = ttk.Entry(group_frame, textvariable=self.groups_config)
        groups_entry.pack(fill=tk.X, pady=(0, 5))
        
        # Example buttons
        example_frame = ttk.Frame(group_frame)
        example_frame.pack(fill=tk.X)
        
        ttk.Button(example_frame, text="Walk Cycle Example", 
                   command=lambda: self.groups_config.set("0:0-3:0.1,0.15,0.1,0.15")).pack(side=tk.LEFT, padx=(0, 5))
        ttk.Button(example_frame, text="Multi-Group Example", 
                   command=lambda: self.groups_config.set("0:0-3:0.1,0.1,0.1,0.1;1:4-7:0.05,0.05,0.05,0.05")).pack(side=tk.LEFT)
        
        # Sprite Settings Section
        settings_frame = ttk.LabelFrame(main_frame, text="Sprite Settings", padding=10)
        settings_frame.pack(fill=tk.X, pady=(0, 10))
        
        # Sprite type and text format
        settings_row1 = ttk.Frame(settings_frame)
        settings_row1.pack(fill=tk.X, pady=(0, 10))
        
        ttk.Label(settings_row1, text="Sprite Type:").pack(side=tk.LEFT)
        self.sprite_type = tk.IntVar(value=2)
        type_combo = ttk.Combobox(settings_row1, textvariable=self.sprite_type,
                                 values=[0, 1, 2, 3, 4], state="readonly", width=5)
        type_combo.pack(side=tk.LEFT, padx=(5, 20))
        
        ttk.Label(settings_row1, text="Text Format:").pack(side=tk.LEFT)
        self.text_format = tk.IntVar(value=0)
        format_combo = ttk.Combobox(settings_row1, textvariable=self.text_format,
                                   values=[0, 1, 2, 3], state="readonly", width=5)
        format_combo.pack(side=tk.LEFT, padx=(5, 0))
        
        # Transparency
        self.preserve_transparency = tk.BooleanVar(value=True)
        ttk.Checkbutton(settings_frame, text="Preserve Transparency", 
                       variable=self.preserve_transparency).pack(anchor=tk.W)
        
        # Convert Button
        convert_frame = ttk.Frame(main_frame)
        convert_frame.pack(fill=tk.X, pady=(0, 10))
        
        self.convert_button = ttk.Button(convert_frame, text="Convert to Sprite", 
                                        command=self.convert_image, style="Accent.TButton")
        self.convert_button.pack(fill=tk.X)
        
        # Progress Bar
        self.progress = ttk.Progressbar(main_frame, mode='indeterminate')
        self.progress.pack(fill=tk.X, pady=(0, 10))
        
        # Log Section
        log_frame = ttk.LabelFrame(main_frame, text="Conversion Log", padding=10)
        log_frame.pack(fill=tk.BOTH, expand=True)
        
        self.log_text = scrolledtext.ScrolledText(log_frame, height=10, state=tk.DISABLED)
        self.log_text.pack(fill=tk.BOTH, expand=True)
        
        ttk.Button(log_frame, text="Clear Log", command=self.clear_log).pack(anchor=tk.E, pady=(5, 0))
        
    def show_drag_drop_info(self):
        messagebox.showinfo("Drag & Drop", 
                           "You can drag and drop image files directly onto this window!")
        
    def on_drop(self, event):
        files = self.root.tk.splitlist(event.data)
        if files:
            file_path = files[0]
            if file_path.lower().endswith(('.png', '.jpg', '.jpeg', '.bmp', '.tiff', '.tga')):
                self.file_path.set(file_path)
                self.load_image_preview()
            else:
                messagebox.showerror("Error", "Please select a valid image file!")
    
    def browse_file(self):
        filename = filedialog.askopenfilename(
            title="Select Image File",
            filetypes=[
                ("Image files", "*.png *.jpg *.jpeg *.bmp *.tiff *.tga"),
                ("PNG files", "*.png"),
                ("JPEG files", "*.jpg *.jpeg"),
                ("All files", "*.*")
            ]
        )
        if filename:
            self.file_path.set(filename)
            self.load_image_preview()
    
    def browse_output(self):
        filename = filedialog.asksaveasfilename(
            title="Save Sprite As",
            defaultextension=".spr",
            filetypes=[
                ("Sprite files", "*.spr"),
                ("All files", "*.*")
            ]
        )
        if filename:
            self.output_path.set(filename)
    
    def load_image_preview(self):
        try:
            if not self.file_path.get():
                return
                
            self.current_image = Image.open(self.file_path.get())
            
            # Auto-fill dimensions
            if not self.frame_width.get():
                self.frame_width.set(str(self.current_image.width))
            if not self.frame_height.get():
                self.frame_height.set(str(self.current_image.height))
            
            # Create preview
            preview_size = (200, 150)
            preview_img = self.current_image.copy()
            preview_img.thumbnail(preview_size, Image.Resampling.LANCZOS)
            
            self.preview_image = ImageTk.PhotoImage(preview_img)
            self.preview_label.config(image=self.preview_image, 
                                    text=f"Image: {self.current_image.width}x{self.current_image.height}")
            
            self.log(f"Loaded image: {self.file_path.get()}")
            self.log(f"Dimensions: {self.current_image.width}x{self.current_image.height}")
            
        except Exception as e:
            messagebox.showerror("Error", f"Failed to load image: {e}")
            self.log(f"Error loading image: {e}")
    
    def auto_dimensions(self):
        if self.current_image:
            self.frame_width.set(str(self.current_image.width))
            self.frame_height.set(str(self.current_image.height))
    
    def log(self, message):
        self.log_text.config(state=tk.NORMAL)
        self.log_text.insert(tk.END, f"{message}\n")
        self.log_text.see(tk.END)
        self.log_text.config(state=tk.DISABLED)
        self.root.update_idletasks()
    
    def clear_log(self):
        self.log_text.config(state=tk.NORMAL)
        self.log_text.delete(1.0, tk.END)
        self.log_text.config(state=tk.DISABLED)
    
    def validate_inputs(self):
        if not self.file_path.get():
            messagebox.showerror("Error", "Please select an input image file!")
            return False
        
        if not os.path.exists(self.file_path.get()):
            messagebox.showerror("Error", "Input file does not exist!")
            return False
        
        try:
            if self.frame_width.get():
                width = int(self.frame_width.get())
                if width <= 0:
                    raise ValueError("Width must be positive")
        except ValueError:
            messagebox.showerror("Error", "Frame width must be a positive integer!")
            return False
        
        try:
            if self.frame_height.get():
                height = int(self.frame_height.get())
                if height <= 0:
                    raise ValueError("Height must be positive")
        except ValueError:
            messagebox.showerror("Error", "Frame height must be a positive integer!")
            return False
        
        try:
            if self.frame_count.get():
                count = int(self.frame_count.get())
                if count <= 0:
                    raise ValueError("Count must be positive")
        except ValueError:
            messagebox.showerror("Error", "Frame count must be a positive integer!")
            return False
        
        return True
    
    def convert_image(self):
        if not self.validate_inputs():
            return
        
        # Disable convert button and start progress
        self.convert_button.config(state=tk.DISABLED)
        self.progress.start()
        
        # Run conversion in separate thread
        thread = threading.Thread(target=self.do_conversion)
        thread.daemon = True
        thread.start()
    
    def do_conversion(self):
        try:
            # Prepare parameters
            input_path = self.file_path.get()
            output_path = self.output_path.get() if self.output_path.get() else None
            
            frame_width = int(self.frame_width.get()) if self.frame_width.get() else None
            frame_height = int(self.frame_height.get()) if self.frame_height.get() else None
            frame_count = int(self.frame_count.get()) if self.frame_count.get() else None
            
            # Create custom log function for converter
            def gui_log(message):
                self.root.after(0, lambda: self.log(message))
            
            # Temporarily replace print with our log function
            original_print = print
            import builtins
            builtins.print = gui_log
            
            try:
                # Convert
                result_path = self.converter.convert_image_to_sprite(
                    input_path=input_path,
                    output_path=output_path,
                    sprite_type=self.sprite_type.get(),
                    text_format=self.text_format.get(),
                    frame_width=frame_width,
                    frame_height=frame_height,
                    frames_layout=self.frame_layout.get(),
                    frames_count=frame_count,
                    preserve_transparency=self.preserve_transparency.get(),
                    group_config=self.groups_config.get() if self.groups_config.get() else None
                )
                
                # Success
                self.root.after(0, lambda: self.conversion_complete(result_path))
                
            finally:
                # Restore original print
                builtins.print = original_print
                
        except Exception as e:
            error_msg = str(e)
            self.root.after(0, lambda: self.conversion_error(error_msg))
    
    def conversion_complete(self, result_path):
        self.progress.stop()
        self.convert_button.config(state=tk.NORMAL)
        
        self.log("✓ Conversion completed successfully!")
        messagebox.showinfo("Success", f"Sprite created successfully!\n\nSaved as: {result_path}")
    
    def conversion_error(self, error_msg):
        self.progress.stop()
        self.convert_button.config(state=tk.NORMAL)
        
        self.log(f"✗ Conversion failed: {error_msg}")
        messagebox.showerror("Conversion Error", f"Failed to convert image:\n\n{error_msg}")

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
    
    # Group options with intervals
    parser.add_argument('--groups', type=str, 
                       help='Group configuration: "group_id:frame_range:intervals;..." (e.g., "0:0-3:0.1,0.2,0.1,0.15;1:4-7:0.05,0.05,0.05,0.05")')
    
    # Transparency options
    parser.add_argument('--no-transparency', action='store_true', help='Disable transparency handling')
    
    args = parser.parse_args()
    
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
            group_config=args.groups
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

def run_gui():
    """Run the GUI version of the sprite converter."""
    try:
        root = tk.Tk()
        
        # Try to set a nice theme
        try:
            style = ttk.Style()
            style.theme_use('clam')
        except:
            pass  # Use default theme if clam is not available
        
        app = SpriteConverterGUI(root)
        
        # Center window on screen
        root.update_idletasks()
        x = (root.winfo_screenwidth() // 2) - (root.winfo_width() // 2)
        y = (root.winfo_screenheight() // 2) - (root.winfo_height() // 2)
        root.geometry(f"+{x}+{y}")
        
        root.mainloop()
        
    except ImportError as e:
        print("GUI dependencies not available. Please install required packages:")
        print("pip install pillow tkinter")
        print(f"Error: {e}")
    except Exception as e:
        print(f"Error running GUI: {e}")

if __name__ == "__main__":
    if len(sys.argv) == 1:
        # No arguments - run GUI
        run_gui()
    elif len(sys.argv) == 2 and sys.argv[1] in ['--gui', '-g']:
        # Explicit GUI request
        run_gui()
    elif len(sys.argv) == 2 and sys.argv[1] in ['--help', '-h']:
        # Show help for both modes
        print("Sprite Converter - Image to .spr format")
        print()
        print("Usage:")
        print("  python conv_img_to_spr.py                    # Run GUI version")
        print("  python conv_img_to_spr.py --gui              # Run GUI version")
        print("  python conv_img_to_spr.py [options] <file>   # Run command line version")
        print()
        print("Command line examples:")
        print("  python conv_img_to_spr.py image.png")
        print("  python conv_img_to_spr.py sprite_sheet.png --frames-layout horizontal -w 64 --height 64")
        print("  python conv_img_to_spr.py sprite_sheet.png --groups '0:0-3:0.1,0.1,0.1,0.1;1:4-7:0.05'")
    else:
        # Command line mode
        main()