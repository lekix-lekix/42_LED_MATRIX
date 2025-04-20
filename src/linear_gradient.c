#include "../include/include.h"

void draw_linear_gradient(t_mlx *window, float color)
{
    t_img *img;

    float color_inc = (255 / G_WIDTH) / 15;

    img = init_img(window);
    for (int i = 0; i < G_WIDTH; i++)
    {
        for (int j = 0; j < G_HEIGHT; j++)
            draw_cell(img, i, j, wheel(color));
        color += color_inc;
    }
    push_img(img, window);
}

void linear_loop(t_mlx *window)
{
    int frame = 0;
    float color = 0;
    int speed = 400;

    while (1)
    {
        usleep(10000);
        color = ((frame % speed) / (float)speed) * 255;
        draw_linear_gradient(window, color);
        frame++;
    }
}
