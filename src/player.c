#include "game.h"
#include <stdio.h>
#include <math.h>

int abs (int n) {return n > 0 ? n : n * -1;}

// ! remove before eval !!!!
// This function uses DDA to detemine the player's vision
// Where FOV is the range the player can see
void draw_player_line(t_game *game, int const player_size)
{
    int const line_len = 100;

    // I'll assume for now that the player can only see forward
    int xs, ys, xe, ye;

    xs = WIDTH / 2 + game->player.pos.x * PIXEL_SCALE + player_size / 2;
    ys = HEIGHT / 2 + game->player.pos.y * PIXEL_SCALE + player_size / 2;


    fprintf(stderr, "player pos x:%d y:%d ray start x:%d y:%d\n",
                        (int)game->player.pos.x,
                        (int)game->player.pos.y, xs , ys);
    xe = xs + (cos(game->player.r) * line_len);
    ye = ys + (sin(game->player.r) * line_len);

    int dx,dy;
    dx = xe - xs;
    dy = ye - ys;

    int steps = (dx) > abs(dy) ? abs(dx) : abs(dy);

    if (!steps) return;

    float incX, incY;
    incX = (float)dx / steps;
    incY = (float)dy / steps;

    int i = 0;
    float xb, yb;
    xb = xs;
    yb = ys;
    while (i < steps)
    {
        if (xb > 0 && yb > 0 && xb < WIDTH && yb < HEIGHT)
            mlx_put_pixel(game->frame, xb, yb, 0x00FF00FF);
        xb += incX;
        yb += incY;
        i++;
    }
}

void update_player_pos(t_game *game)
{
    int base_px = WIDTH / 2 + game->player.pos.x * PIXEL_SCALE;
    int base_py = HEIGHT / 2 + game->player.pos.y * PIXEL_SCALE;

    int const player_size = 20;
    int i = 0;
    int j = 0;
    int px = 0;
    int py = 0;
    while (i < player_size)
    {
        py = base_py +  i;
        while (j < player_size)
        {
            px = base_px + j;
            if ((px < WIDTH && py < HEIGHT) && (px > 0 && py > 0))
                mlx_put_pixel(game->frame, px, py, 0x4287f5);
            j++;
        }
        j = 0;
        i++;
    }
    draw_player_line(game, player_size);
}
