#include "../include/include.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

bool is_in_cell_x(int j, int x)
{
    if (x == 0 && j < GRID_X - 1)
        return (true);
    return (j >= (x * (GRID_X - 1)) && j < ((x + 1) * (GRID_X - 1)));
}

bool is_in_cell_y(int i, int y)
{
    if (y == 0 && i < GRID_Y - 1)
        return (true);
    return (i >= (y * (GRID_Y - 1)) && i < ((y + 1) * (GRID_Y - 1)));
}

void draw_cell(t_img *img, int x, int y, int color)
{
    for (int i = 0; i < W_HEIGHT; i++)
    {
        for (int j = 0; j < W_WIDTH; j++)
        {
            if (is_in_cell_x(j, x) && is_in_cell_y(i, y))
                img_pix_put(img, j, i, color);
        }
    }
}

void draw_grid(t_img *img, int color)
{
    for (int i = 0; i < W_HEIGHT; i++)
    {
        for (int j = 0; j < W_WIDTH; j++)
        {
            if (j % (GRID_Y - 1) == 0 || i % (GRID_X - 1) == 0)
                img_pix_put(img, j, i, color);
        }
    }
}

bool check_if_alive(int tab[15][20], int x, int y)
{
    int i;
    int j;
    int alive = 0;
    
    i = y - 1;
    if (i < 0)
        i = 0;
    while (i <= y + 1)
    {
        j = x - 1;
        if (j < 0)
            j = 0;
        while (j <= x + 1)
        {
            if (tab[i][j] == true)
                alive++;
            j++;
        }
        i++;
    }
    if (alive != 0)
        alive -= 1;
    printf("alive = %d\n", alive);
    if (alive == 2 || alive == 3)
        return (true);
    return (false);
}

int main ()
{
    t_mlx window = start_mlx(W_WIDTH, W_HEIGHT);
    t_img *img = init_img(&window);
    
    int conway_tab[G_HEIGHT][G_WIDTH] = {
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    // while (1)
    // {
        // usleep(10000);
        // conway(conway_tab);
        for (int i = 0; i < G_HEIGHT; i++)
        {
            for (int j = 0; j < G_WIDTH; j++)
            {
                // printf("i = %d j = %d\n", i, j);
                if (conway_tab[i][j] && check_if_alive(conway_tab, j, i))
                    draw_cell(img, j, i, 0xFFFFFF);
                else
                    draw_cell(img, j, i, 0x000000);
            }
        }
        
        // draw_cell(img, 10, 10, 0xFFFFFF);
        draw_grid(img, 0xFF);
        mlx_put_image_to_window(window.mlx_ptr, window.win_ptr, img->img_ptr, 0, 0);
        // i % 2 == 0 ? draw_cell(img, 1, 1, 0xFFFFFF) : draw_cell(img, 1, 1, 0x000000);
        // mlx_put_image_to_window(window.mlx_ptr, window.win_ptr, img->img_ptr, 0, 0);
        // i++;
    // }
    mlx_loop(window.mlx_ptr);
}