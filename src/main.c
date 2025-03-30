#include "../include/include.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

void draw_cell(t_img *img, int x, int y, int color)
{
    int x_limit = (x + 1) * (GRID_X - 1);
    int y_limit = (y + 1) * (GRID_Y - 1);
    int i = y * (GRID_Y - 1);
    int j;

    while (i < y_limit)
    {
        j = x * (GRID_X - 1);
        while (j < x_limit)
        {
            img_pix_put(img, j, i, color);
            j++;
        }
        i++;
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

void check_if_alive(__uint8_t tab[15][20], __uint8_t next_tab[15][20], int x_tab, int y_tab, t_img *img)
{
    int x;
    int y;
    int alive = 0;
    
    y = y_tab - 1;
    if (y < 0)
        y = 0;
    while (y <= y_tab + 1)
    {
        if (y >= G_HEIGHT)
            break ;
        x = x_tab - 1;
        if (x < 0)
            x = 0;
        while (x <= x_tab + 1)
        {
            if (x >= G_WIDTH)
                break ;
            // if (x_tab == 7 && y_tab == 8)
                // printf("x = %d y = %d\n", x, y);
            if (tab[y][x] && !(x == x_tab && y == y_tab))
            {
                // if (x_tab == 7 && y_tab == 8)
                    // printf("alive\n");
                alive++;
            }
            x++;
        }
        y++;
    }
    // if (alive != 0)
    //     alive -= 1;
    // if (x_tab == 7 && y_tab == 8)
    //     printf("alive = %d\n", alive);
    if (tab[y_tab][x_tab] && alive < 2)
    {
        // if (x_tab == 7 && y_tab == 8)
            // printf("la %d\n", alive);
        next_tab[y_tab][x_tab] = 0;
        draw_cell(img, x_tab, y_tab, 0x000000);
    }
    else if (tab[y_tab][x_tab] && (alive == 2 || alive == 3)) 
    {
        // if (x_tab == 7 && y_tab == 8)
            // printf("ici %d\n", alive);
        next_tab[y_tab][x_tab] = 1;
        draw_cell(img, x_tab, y_tab, 0xFFFFFF);
    }
    else if (tab[y_tab][x_tab] && alive > 3)
    {
        next_tab[y_tab][x_tab] = 0;
        draw_cell(img, x_tab, y_tab, 0x000000);
    }
    else if (!tab[y_tab][x_tab] && alive == 3)
    {
        next_tab[y_tab][x_tab] = 1;
        draw_cell(img, x_tab, y_tab, 0xFFFFFF);
    }
}

void print_tab(__uint8_t tab[15][20])
{
    for (int i = 0; i < G_HEIGHT; i++)
    {
        printf("%d ", i);
        if (i < 10)
            printf(" ");
        for (int j = 0; j < G_WIDTH; j++)
            printf("%d ", tab[i][j]);
        printf("\n");
    }
}

void copy_tab(__uint8_t tab[15][20], __uint8_t buffer_tab[15][20])
{
    for (int y = 0; y < G_HEIGHT; y++)
    {
        for (int x = 0; x < G_WIDTH; x++)
            tab[y][x] = buffer_tab[y][x];
    }
}

void conway(__uint8_t tab[15][20], t_img *img)
{
    __uint8_t next_tab[15][20] = {0};
    
    // print_tab(buffer_tab);
    for (int y = 0; y < G_HEIGHT; y++)
    {
        for (int x = 0; x < G_WIDTH; x++)
            check_if_alive(tab, next_tab, x, y, img);
    }
    copy_tab(tab, next_tab);
    // print_tab(tab);
    // printf("yo\n");
}

int main ()
{
    t_mlx window = start_mlx(W_WIDTH, W_HEIGHT);
    t_img *img;
    
    __uint8_t conway_tab[G_HEIGHT][G_WIDTH] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    while (1)
    {
        // print_tab(conway_tab);
        usleep(100000);
        img = init_img(&window);
        conway(conway_tab, img);
        // printf("hey\n");
        // for (int y = 0; y < G_HEIGHT; y++)
        // {
        //     for (int x = 0; x < G_WIDTH; x++)
        //     {
        //         if (conway_tab[x][y])
        //             draw_cell(img, x, y, 0xFFFFFF);
        //         else
        //             draw_cell(img, x, y, 0x000000);
        //     }
        // }
        // draw_cell(img, 19, 14, 0xFFFFFF);
        draw_grid(img, 0xFF);
        mlx_put_image_to_window(window.mlx_ptr, window.win_ptr, img->img_ptr, 0, 0);
        mlx_destroy_image(window.mlx_ptr, img->img_ptr);
        free(img);
        // mlx_loop(window.mlx_ptr);
    }
}