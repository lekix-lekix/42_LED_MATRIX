#include "../include/include.h"

float square(float nb)
{
    return (nb * nb);
}

float get_norm_distance(t_cell *cell, t_cell *center, float max_distance)
{
    float distance_squared = square(cell->x - center->x) + square(cell->y - center->y);
    return (distance_squared / square(max_distance));
}

void radial_gradient(t_mlx *window, int frame)
{
    t_img *img = init_img(window);
    
    t_cell cell;
    t_cell center = {9, 7};
    float norm_distance;
    float max_distance = 12.5;
    float wave;
    
    for (int i = 0; i < G_WIDTH; i++)
    {
        for (int j = 0; j < G_HEIGHT; j++)
        {
            cell.x = i;
            cell.y = j;
            norm_distance = get_norm_distance(&cell, &center, max_distance);
            wave = (sin(norm_distance / 1.0f) - (frame / 500.0f) + 1) / 2;
            draw_cell(img, i, j, wheel(wave * 255));
        }
    }
    push_img(img, window);
}

void radial_loop(t_mlx *window)
{
    int frame = 0;
    
    while (1)
    {
        radial_gradient(window, frame);
        frame++;
    }

}