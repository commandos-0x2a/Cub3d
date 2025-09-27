# Multi-Frame Sprite Converter - Usage Examples

This document demonstrates how to use the enhanced sprite converter with multi-frame support.

## New Features

The sprite converter now supports extracting multiple frames from sprite sheets and creating animated sprites.

### Frame Layout Options

1. **single** - Single frame (default behavior)
2. **horizontal** - Frames arranged in a horizontal strip (left to right)
3. **vertical** - Frames arranged in a vertical strip (top to bottom)
4. **grid** - Frames arranged in a 2D grid
5. **auto** - Automatically detect layout based on image dimensions

## Command Line Examples

### Basic Multi-Frame Usage
```bash
# Convert horizontal sprite strip (4 frames, 64x64 each)
python conv_img_to_spr.py sprite_strip.png --frames-layout horizontal -w 64 --height 64

# Convert vertical sprite strip with specific frame count
python conv_img_to_spr.py vertical_strip.png --frames-layout vertical --frames-count 8 -w 32 --height 32

# Convert 2D grid sprite sheet
python conv_img_to_spr.py sprite_grid.png --frames-layout grid --frames-count 16 -w 48 --height 48

# Auto-detect frame layout
python conv_img_to_spr.py animation.png --frames-layout auto -w 64 --height 64
```

### Advanced Examples
```bash
# Create additive particle animation
python conv_img_to_spr.py fire_frames.png --frames-layout horizontal -w 32 --height 32 -f 1 -o fire.spr

# Create oriented character animation
python conv_img_to_spr.py character_walk.png --frames-layout horizontal --frames-count 8 -w 64 --height 64 -t 3
```

## Test Files Created

### Horizontal Animation (test_animation.png)
- 4 frames arranged horizontally
- Each frame: 64x64 pixels
- Different colored circles growing in size
- Command: `python conv_img_to_spr.py test_animation.png --frames-layout horizontal -w 64 --height 64`

### Vertical Animation (test_vertical.png)
- 4 frames arranged vertically
- Each frame: 48x48 pixels
- Colored squares moving horizontally
- Command: `python conv_img_to_spr.py test_vertical.png --frames-layout vertical -w 48 --height 48`

### Grid Animation (test_grid.png)
- 4 frames in 2x2 grid
- Each frame: 32x32 pixels
- Different colored rectangles
- Command: `python conv_img_to_spr.py test_grid.png --frames-layout grid -w 32 --height 32 --frames-count 4`

## Interactive Mode

The interactive mode now includes multi-frame options:

```
=== Image to Sprite Converter (Multi-Frame Support) ===

Enter image file path: sprite_sheet.png
Original image size: 256x64
Frame width (default: 256): 64
Frame height (default: 64): 64

Frame Layout Options:
  single     - Single frame (default)
  horizontal - Frames arranged horizontally (sprite strip)
  vertical   - Frames arranged vertically
  grid       - Frames in a 2D grid
  auto       - Auto-detect based on image dimensions
Frame layout (default: single): horizontal
Number of frames (optional, auto-detect if blank): 4
Output path (default: sprite_sheet.spr): animation.spr
```

## Output Format

Multi-frame sprites maintain compatibility with the Half-Life 1 sprite format:

### File Structure
```
Sprite Header (46 bytes)
├── ID: "IDSP"
├── Version: 2
├── Frame Count: N
└── Other properties...

Palette (768 bytes)
├── 256 RGB colors

Frame 1:
├── Frame Header (20 bytes)
└── Pixel Data (width × height bytes)

Frame 2:
├── Frame Header (20 bytes)
└── Pixel Data (width × height bytes)

...

Frame N:
├── Frame Header (20 bytes)
└── Pixel Data (width × height bytes)
```

## Frame Extraction Logic

### Horizontal Layout
```
Original: [Frame1][Frame2][Frame3][Frame4]
Result: 4 separate frames, each width/4 × height
```

### Vertical Layout
```
Original: [Frame1]
          [Frame2]
          [Frame3]
          [Frame4]
Result: 4 separate frames, each width × height/4
```

### Grid Layout
```
Original: [Frame1][Frame2]
          [Frame3][Frame4]
Result: 4 separate frames from 2×2 grid
```

### Auto Detection
- If image width > height: use horizontal layout
- If image height > width: use vertical layout
- Equal dimensions: use single frame

## Validation Results

All generated multi-frame sprites pass validation:

```bash
# Test with Python validator
python sprite_reader.py animation.spr
# Output: "✓ Sprite format validation PASSED"

# Test with C validator
./test_sprite_reader animation.spr
# Output: "✓ Sprite loaded successfully!"
```

## Performance Notes

- Multi-frame sprites share a single 256-color palette across all frames
- Palette is generated from a combined sample of all frames
- Memory usage scales linearly with frame count
- File size = Header + Palette + (Frame_Count × Frame_Size)

## Integration with Game Engines

Multi-frame sprites can be used for:

1. **Character Animations**: Walking, running, idle cycles
2. **Particle Effects**: Fire, smoke, explosions with additive blending
3. **UI Animations**: Loading spinners, button states
4. **Environmental Effects**: Water, wind, flickering lights
5. **Weapon Effects**: Muzzle flashes, impact effects

Example game code usage:
```c
// Load multi-frame sprite
load_sprite_file("animations/fire.spr", &sprite, palette, &frame_header, &pixel_data);
printf("Loaded animation with %d frames\n", sprite.nb_frame);

// Animate by cycling through frames
current_frame = (current_frame + 1) % sprite.nb_frame;
```

## Common Use Cases

### Sprite Sheets from Animation Software
Most animation software exports sprite sheets in standard layouts:

- **Aseprite**: Horizontal strips (use `--frames-layout horizontal`)
- **Photoshop**: Grid layouts (use `--frames-layout grid`)
- **GIMP**: Various layouts (use `--frames-layout auto`)

### Game Asset Pipelines
```bash
# Batch convert multiple animations
for file in assets/animations/*.png; do
    python conv_img_to_spr.py "$file" --frames-layout auto -w 64 --height 64
done
```

The enhanced sprite converter now provides a complete solution for creating both static and animated sprites for game development.