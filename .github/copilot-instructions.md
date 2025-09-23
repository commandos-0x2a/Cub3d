# Cub3D AI Coding Instructions

## Architecture Overview

Cub3D is a raycasting 3D game engine built with MLX42 graphics library, featuring:

- **Core Engine**: Raycasting algorithm with DDA (Digital Differential Analyzer) for wall detection
- **Map System**: Grid-based world with `.cub` file parsing for textures, colors, and layout
- **Sprite System**: Custom Half-Life 1 compatible sprite format with multi-frame animation support
- **Rendering Pipeline**: Real-time frame buffer manipulation with texture mapping and fisheye correction

## Project Structure

```
src/
├── main.c              # MLX42 initialization, game loop setup
├── render.c            # Frame rendering, floor/ceiling drawing
├── player.c            # Raycasting logic, wall texture mapping
├── controls.c          # Input handling, collision detection
├── utils.c             # Helper functions
├── map/                # Map parsing and validation
└── image/              # Sprite loading and image utilities
```

## Critical Constants (include/utils.h)

```c
#define WIDTH 1080      // Default window width
#define HEIGHT 720      // Default window height  
#define FOV 60          // Field of view in degrees
#define TILE_SIZE 1     // Map grid scale factor
```

## Core Data Structures

**t_game**: Central game state containing MLX context, map, player, textures array, and timing
**t_map**: Contains texture paths, colors, and grid data from `.cub` files
**t_player**: Position (x,y), rotation angle, and movement speed
**t_ray_hit**: Ray collision data with distance, wall texture coordinate, and wall direction

## Build System

```bash
# Main build - requires MLX42 submodule
make

# Sprite tools (separate build system)
make -f sprite_makefile        # C sprite reader/validator
python conv_img_to_spr.py      # Python sprite converter
```

## MLX42 Integration Patterns

- **Hook Pattern**: Use `mlx_loop_hook()` for render and input functions
- **Image Management**: Create frame buffer with `mlx_new_image()`, manipulate with `mlx_put_pixel()`  
- **Texture Loading**: Load PNG textures into `mlx_texture_t*` array indexed by wall direction
- **Window Events**: Handle resize via `resize_hook()` which updates ray count and dimensions

## Raycasting Implementation

The engine uses DDA algorithm in `cast_ray()`:
1. Calculate ray direction from player angle
2. Step through grid using delta distances  
3. Detect wall hits and calculate perpendicular distance
4. Return ray hit data with texture coordinates

**Fisheye Correction**: `corrected_distance = distance * cos(angle_offset)` in `draw_wall()`

## Map File Format

`.cub` files define:
- `NO/SO/WE/EA` texture paths for each wall direction
- `F/C` RGB values for floor/ceiling colors  
- Grid layout using `0` (empty), `1` (wall), `N/S/E/W` (player start)

## Sprite System

Two-part sprite pipeline:
1. **Python Converter**: `conv_img_to_spr.py` converts images to custom `.spr` format
   - Multi-frame support: `--frames-layout horizontal/vertical/grid`
   - Half-Life 1 compatible format with 256-color palette
2. **C Reader**: Functions in `src/image/sprite_reader.c` load sprites into game

## Development Workflows

**Testing Sprites**:
```bash
# Convert image to sprite
python conv_img_to_spr.py image.png --frames-layout horizontal -w 64 --height 64

# Validate sprite format
./test_sprite_reader sprite.spr
```

**Map Testing**: Place `.cub` files in `maps/` directory, run `./cub3d maps/test.cub`

**Debug Patterns**: 
- Use `printf()` for coordinate debugging (many examples in `controls.c`)
- FPS counter available in `t_debug` structure
- Minimap rendering in `update_minimap()` for position visualization

## Common Patterns

**Vector Math**: Use `t_vector` for 2D coordinates, trigonometric functions for rotation
**Collision Detection**: `wall_collision()` checks map grid before applying movement
**Texture Mapping**: Calculate `tex_x` from `ray.wall_x`, `tex_y` from wall height ratio
**Memory Management**: Manual allocation/free for sprites and map data

## Integration Points

- **MLX42**: Graphics rendering and input handling
- **libft**: Custom C library for string/memory utilities  
- **Python Tools**: Asset conversion pipeline for sprites
- **CMake**: MLX42 dependency building

Use texture array indexing (`game->texture[WALL_NORTH]`) for directional textures. Always validate map boundaries and handle edge cases in raycasting to prevent segfaults.