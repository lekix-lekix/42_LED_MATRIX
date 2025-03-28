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
