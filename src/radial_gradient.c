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

float branches = -10.0f;
float pixellization = 0.1f; // 1,
float spiral_speed = 0.01f;
int   mode = 0;

void radial_gradient(t_mlx *window, int frame)
{
    t_img *img = init_img(window);
    
    t_cell cell;
    t_cell center = {9, 7};
    float max_distance = 12.5;
    float color;
    
    float anim_speed = 0.003f;
    // /* float  */branches = -10.0f;      // 5, 3, 10 & +
    float form = -2.0f;         // -2, -200

    for (int i = 0; i < G_WIDTH; i++)
    {
        for (int j = 0; j < G_HEIGHT; j++)
        {
            cell.x = i;
            cell.y = j;
            float angle = atan2(cell.y - center.y, cell.x - center.x);
            float distance = get_norm_distance(&cell, &center, max_distance);
            float r = distance * (form + (pixellization * sin(branches * angle)));
            color = fmodf((r / max_distance + frame * anim_speed) * 255.0f, 255.0f);
            draw_cell(img, i, j, wheel(color));
        }
    }
    push_img(img, window);
}

void radial_gradient_round(t_mlx *window, int frame)
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

void spiral(t_mlx *window, int frame)
{
    t_img *img = init_img(window);
    
    t_cell cell;
    t_cell center = {9, 7};
    float max_distance = 12.5;
    float color;
    
    // float anim_speed = 0.003f;
    // /* float  */branches = -10.0f;      // 5, 3, 10 & +
    // float form = -2.0f;         // -2, -200
    (void) frame;

    for (int i = 0; i < G_WIDTH; i++)
    {
        for (int j = 0; j < G_HEIGHT; j++)
        {
            cell.x = i;
            cell.y = j;
            float angle = atan2(cell.y - center.y, cell.x - center.x);
            float distance = get_norm_distance(&cell, &center, max_distance);
            float spiral = angle + distance / 2.5f + frame * spiral_speed;
            // spiral += frame * spiral_speed; // speed
            color = fmodf(spiral * 50.0f, 255.0f);
            draw_cell(img, i, j, wheel(color));
        }
    }
    push_img(img, window);
}

int handle_keys(int key)
{
    if (key == 65307)
        exit(0);
    else if (key == UP_KEY && mode == 0)
        branches += 0.1f;
    else if (key == DOWN_KEY && mode == 0)
        branches -= 0.1f;
    else if (key == 119 && mode == 0)
        pixellization += 0.1f;
    else if (key == 115 && mode == 0)
        pixellization -= 0.1f;
    else if (key == UP_KEY && mode == 2)
        spiral_speed += 0.01f;
    else if (key == DOWN_KEY && mode == 2)
        spiral_speed -= 0.01f;
    else if (key == 65361)
        mode = (mode + 1 ) % 3;
    // printf("key = %d\n",key);
    // printf("form = %f\n", *form);
    return 0;
}

int start_radial(t_mlx *window)
{
    static int frame = 0;
    
    if (mode == 0)
        radial_gradient(window, frame);
    else if (mode == 1)
        radial_gradient_round(window, frame);
    else
        spiral(window, frame);
    frame++;
    return (0); 
}

int radial_loop(t_mlx *window)
{
    while (1)
    {
        mlx_hook(window->win_ptr, 2, 1L << 0, handle_keys, &pixellization);
        mlx_loop_hook(window->mlx_ptr, &start_radial, window);
        mlx_loop(window->mlx_ptr);    
    }
}