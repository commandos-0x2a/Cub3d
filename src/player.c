#include "game.h"
#include <stdio.h>
#include <math.h>

#define PI 3.14159265359

// Cast ray using DDA
t_ray_hit cast_ray(t_game *game, double angle, int draw_ray)
{
    float px = game->player.pos.x;
    float py = game->player.pos.y;
    float dir_x = cos(angle);
    float dir_y = sin(angle);

    int map_x = (int)px;
    int map_y = (int)py;

    // Avoid division by zero by making the another number comically large
    float delta_dist_x = (dir_x == 0) ? 1e30 : fabs(1.0 / dir_x);
    float delta_dist_y = (dir_y == 0) ? 1e30 : fabs(1.0 / dir_y);

    float side_dist_x, side_dist_y;
    float step_x, step_y;

    // Initialize step and sideDist
    if (dir_x < 0)
    {
        step_x = -1;
        side_dist_x = (px - map_x) * delta_dist_x;
    }
    else
    {
        step_x = 1;
        side_dist_x = (map_x + 1.0 - px) * delta_dist_x;
    }

    if (dir_y < 0)
    {
        step_y = -1;
        side_dist_y = (py - map_y) * delta_dist_y;
    }
    else
    {
        step_y = 1;
        side_dist_y = (map_y + 1.0 - py) * delta_dist_y;
    }

    int hit = 0;
    int side;

    while (!hit)
    {
        if (side_dist_x < side_dist_y)
        {
            side_dist_x += delta_dist_x;
            map_x += (int)step_x;
            side = 0; // vertical wall
        }
        else
        {
            side_dist_y += delta_dist_y;
            map_y += (int)step_y;
            side = 1; // horizontal wall
        }

        // Check bounds
        if (map_x < 0 || map_x >= game->map->grid.w ||
            map_y < 0 || map_y >= game->map->grid.h)
            break; // out of bounds = hit (out of bounds)

        // Check if wall
        if (game->map->grid.raw[map_y * game->map->grid.w + map_x] == '1')
            hit = 1;
    }

    t_ray_hit ray;

    // Calculate perpendicular wall distance (for fisheye correction)
    if (side == 0)
        ray.distance = (map_x - px + (1 - step_x) / 2) / dir_x;
    else
        ray.distance = (map_y - py + (1 - step_y) / 2) / dir_y;

    // Calculate wall texture coordinate
    if (side == 0)
        ray.wall_x = py + ray.distance * dir_y;
    else
        ray.wall_x = px + ray.distance * dir_x;

    // Keep fractional part only for texture
    ray.wall_x -= floor(ray.wall_x);
    if (ray.wall_x < 0) ray.wall_x += 1.0f;

    ray.is_vertical = side == 0;

    return ray;
}

void draw_wall(t_game *game, t_ray_hit ray_hit, int ray)
{
    int x, y;
    int wall_height;
    int wall_start;
    int wall_end;
    int x_start;
    int x_end;
    double angle_offset;
    float corrected_distance;

    // Fish-eye correction
    angle_offset = (ray - NUM_RAYS / 2) * (FOV * PI / 180.0 / NUM_RAYS);
    corrected_distance = ray_hit.distance * cos(angle_offset);

    // Avoid division by zero
    if (corrected_distance <= 0.1f)
        corrected_distance = 0.1f;

    wall_height = (int)(HEIGHT * TILE_SIZE / corrected_distance);
    if (wall_height > HEIGHT) wall_height = HEIGHT;
    if (wall_height < 1) wall_height = 1;

    wall_start = (HEIGHT - wall_height) / 2;
    wall_end = wall_start + wall_height;

    x_start = (ray * WIDTH) / NUM_RAYS;
    x_end = ((ray + 1) * WIDTH) / NUM_RAYS;

    mlx_texture_t *texture = game->texture[0];
    if (!texture || !texture->pixels)
        return;

    for (x = x_start; x < x_end; x++)
    {
        if (x < 0 || x >= WIDTH) continue;

        for (y = wall_start; y < wall_end; y++)
        {
            if (y < 0 || y >= HEIGHT) continue;

            // Map texture X: get texture column coordinate
            int tex_x = (int)(ray_hit.wall_x * texture->width);
            if (tex_x < 0) tex_x = 0;
            if (tex_x >= (int)texture->width) tex_x = texture->width - 1;

            // Map texture Y: get texture row coordinate
            float tex_y_float = (float)(y - wall_start) / (float)wall_height * texture->height;
            int tex_y = (int)tex_y_float;
            if (tex_y < 0) tex_y = 0;
            if (tex_y >= (int)texture->height) tex_y = texture->height - 1;

            // Get pixel color (RGBA)
            uint8_t *pixel = texture->pixels + (tex_y * texture->width + tex_x) * 4;
            uint32_t color = (pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | pixel[3];

            mlx_put_pixel(game->frame, x, y, color);
        }
    }
}

void draw_player_vision(t_game *game)
{
    int i;
    double fov_rad;
    double sangle;
    double angle_step;
    double current_angle;
    t_ray_hit ray_hit;

    fov_rad = FOV * PI / 180.0;
    sangle = game->player.r - (fov_rad / 2.0);
    angle_step = fov_rad / NUM_RAYS;

    for (i = 0; i < NUM_RAYS; i++)
    {
        current_angle = sangle + i * angle_step;
        ray_hit = cast_ray(game, current_angle, 1);
        draw_wall(game, ray_hit, i);
    }
}

void update_player_pos(t_game *game)
{
    draw_player_vision(game);
}
