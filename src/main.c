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

void check_if_alive(__uint8_t tab[15][20], int x_tab, int y_tab)
{
    int x;
    int y;
    int alive = 0;
    
    y = y_tab - 1;
    if (y < 0)
        y = 0;
    if (y >= G_HEIGHT)
        y = G_HEIGHT - 1;
    while (y <= y_tab + 1)
    {
        x = x_tab - 1;
        if (x < 0)
            x = 0;
        if (x >= G_HEIGHT)
            x = G_HEIGHT - 1;
        while (x <= x_tab + 1)
        {
            if (tab[x][y])
                alive++;
            x++;
        }
        y++;
    }
    if (alive != 0)
        alive -= 1;
    // printf("alive = %d\n", alive);
    if (alive == 2 || alive == 3)
        tab[x_tab][y_tab] = 1;
    else
    {
        // printf("x = %d y = %d\n", x, y);
        tab[x_tab][y_tab] = 0;
    }
}

void conway(__uint8_t tab[15][20])
{
    for (int y = 0; y < G_HEIGHT; y++)
    {
        for (int x = 0; x < G_WIDTH; x++)
            check_if_alive(tab, x, y);
    }
}

int main ()
{
    t_mlx window = start_mlx(W_WIDTH, W_HEIGHT);
    t_img *img;
    
    __uint8_t conway_tab[G_HEIGHT][G_WIDTH] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    while (1)
    {
        usleep(10000);
        img = init_img(&window);
        conway(conway_tab);
        for (int y = 0; y < G_HEIGHT; y++)
        {
            for (int x = 0; x < G_WIDTH; x++)
            {
                if (conway_tab[x][y])
                    draw_cell(img, x, y, 0xFFFFFF);
                else
                    draw_cell(img, x, y, 0x000000);
            }
        }
        draw_grid(img, 0xFF);
        mlx_put_image_to_window(window.mlx_ptr, window.win_ptr, img->img_ptr, 0, 0);
        mlx_destroy_image(window.mlx_ptr, img->img_ptr);
        free(img);
    }
    mlx_loop(window.mlx_ptr);
}