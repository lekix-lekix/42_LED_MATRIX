#include "../include/include.h"

t_mlx window;
t_img *img = NULL;

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

int check_neighbours(__uint8_t tab[15][20], int x_tab, int y_tab)
{
    int x;
    int y;
    int alive = 0;

    for (int i = -1; i <= 1; i++)
    {
        y = (y_tab + i + G_HEIGHT) % G_HEIGHT;
        for (int j = -1; j <= 1; j++)
        {
            x = (x_tab + j + G_WIDTH) % G_WIDTH;
            if (tab[y][x] && !(x == x_tab && y == y_tab))
                alive++;
        }
    }
    return (alive);
}

void check_if_alive(__uint8_t tab[15][20], __uint8_t next_tab[15][20], int x_tab, int y_tab, t_img *img)
{
    int alive = 0;

    alive = check_neighbours(tab, x_tab, y_tab);
    if (tab[y_tab][x_tab] && alive < 2)
    {
        next_tab[y_tab][x_tab] = 0;
        draw_cell(img, x_tab, y_tab, 0x000000);
    }
    else if (tab[y_tab][x_tab] && (alive == 2 || alive == 3)) 
    {
        next_tab[y_tab][x_tab] = 1;
        draw_cell(img, x_tab, y_tab, 0x00FF00);
    }
    else if (tab[y_tab][x_tab] && alive > 3)
    {
        next_tab[y_tab][x_tab] = 0;
        draw_cell(img, x_tab, y_tab, 0x000000);
    }
    else if (!tab[y_tab][x_tab] && alive == 3)
    {
        next_tab[y_tab][x_tab] = 1;
        draw_cell(img, x_tab, y_tab, 0x00FF00);
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

void copy_tab(__uint8_t tab[15][20], __uint8_t next_tab[15][20])
{
    for (int y = 0; y < G_HEIGHT; y++)
    {
        for (int x = 0; x < G_WIDTH; x++)
            tab[y][x] = next_tab[y][x];
    }
}

void conway(__uint8_t tab[15][20], t_img *img)
{
    __uint8_t next_tab[15][20] = {0};
    
    for (int y = 0; y < G_HEIGHT; y++)
    {
        for (int x = 0; x < G_WIDTH; x++)
            check_if_alive(tab, next_tab, x, y, img);
    }
    copy_tab(tab, next_tab);
}

void push_img(t_img *img, t_mlx *window)
{
    mlx_put_image_to_window(window->mlx_ptr, window->win_ptr, img->img_ptr, 0, 0);
    mlx_destroy_image(window->mlx_ptr, img->img_ptr);
    free(img);
}

void draw_tab(__uint8_t tab[15][20])
{
    for (int i = 0; i < G_HEIGHT; i++)
    {
        for (int j = 0; j < G_WIDTH; j++)
        {
            if (tab[i][j])
                draw_cell(img, j, i, ALIVE_COLOR);
            else
                draw_cell(img, j, i, DEAD_COLOR);
        }
    }
    draw_grid(img, GRID_COLOR);
}

int register_click(int key, int x, int y, __uint8_t tab[15][20])
{
    (void) key;
    float pos_x = (float)x / (float)(GRID_X - 1);
    float pos_y = (float)y / (float)(GRID_Y - 1);

    tab[(int)pos_y][(int)pos_x] ^= 1;
    img = init_img(&window);
    draw_tab(tab);
    push_img(img, &window);
    return (0);
}

int launch_simulation(int key, __uint8_t conway_tab[15][20])
{
    if (key != 32) return (-1);
    while (1)
    {
        usleep(100000);
        img = init_img(&window);
        conway(conway_tab, img);
        draw_grid(img, 0xFF);
        push_img(img, &window);
    }
    return (0);
}

int main ()
{
    window = start_mlx(W_WIDTH, W_HEIGHT);    
    __uint8_t conway_tab[G_HEIGHT][G_WIDTH] = {0};

    img = init_img(&window);
    draw_grid(img, 0xFF);
    push_img(img, &window);
    mlx_hook(window.win_ptr, 2, 1L << 0, launch_simulation, conway_tab);
    mlx_hook(window.win_ptr, 4, 1L << 2, register_click, conway_tab);
    mlx_loop(window.mlx_ptr);
}