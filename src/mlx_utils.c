#include "../include/include.h"

t_mlx	start_mlx(int width, int height)
{
	t_mlx window;
    
    window.mlx_ptr = mlx_init();
	if (!window.mlx_ptr)
		exit(-1);
	window.width = width;
	window.height = height;
	window.win_ptr = mlx_new_window(window.mlx_ptr, width, height, "LED_MATRIX");
	if (!window.win_ptr)
        exit(-1);
    return (window);
}

t_img	*init_img(t_mlx *window)
{
	t_img	*img;

	img = malloc(sizeof(t_img));
	if (!img)
        exit(-1);
    img->img_ptr = mlx_new_image(window->mlx_ptr, window->width, window->height);
	if (!img->img_ptr)
        exit(-1);
    img->img_addr = mlx_get_data_addr(img->img_ptr, &img->bpp, &img->line_len,
			&img->endian);
	if (!img->img_addr)
        exit(-1);
    return (img);
}

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

int	*get_pixel_from_img(t_img *img, int x, int y)
{
	char	*pixel;

	pixel = img->img_addr + (y * img->line_len + x * (img->bpp / 8));
	return ((int *)pixel);
}

void	img_pix_put(t_img *img, int x, int y, int color)
{
	char	*pixel;

	pixel = img->img_addr + (y * img->line_len + x * (img->bpp / 8));
	*(int *)pixel = color;
}

void push_img(t_img *img, t_mlx *window)
{
    mlx_put_image_to_window(window->mlx_ptr, window->win_ptr, img->img_ptr, 0, 0);
    mlx_destroy_image(window->mlx_ptr, img->img_ptr);
    free(img);
}