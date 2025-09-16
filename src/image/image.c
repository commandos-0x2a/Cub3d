#include "game.h"

uint32_t    get_pixel_color(mlx_texture_t *texture, int tex_x, int tex_y)
{
    uint8_t *pixel = texture->pixels + (tex_y * texture->width + tex_x) * 4;
	uint32_t color = (pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | pixel[3];
    
    return (color);
}

// uint32_t    get_pixel_color2(t_texture *texture, int tex_x, int tex_y)
// {
//     uint32_t        *color;
//     mlx_texture_t   *ctx;

//     if (texture->type == IT_PNG || texture->type == IT_XPM)
//     {
//         ctx = texture->ctx;
//         uint8_t *pixel = ctx->pixels + (tex_y * ctx->width + tex_x) * 4;
//         color = (pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | pixel[3];
//     }
//     else
//     {
//         color = 0; // TODO: handle sprites
//     }
//     return (color);
// }