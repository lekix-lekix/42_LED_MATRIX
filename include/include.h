#ifndef INCLUDE_H
#define INCLUDE_H

#define W_WIDTH 800
#define W_HEIGHT 600
#define G_WIDTH 20
#define G_HEIGHT 15

// #define GRID_X W_WIDTH / G_WIDTH 
// #define GRID_Y W_HEIGHT / G_HEIGHT
# define GRID_X 40
# define GRID_Y 40

# define ALIVE_COLOR 0x00FF00
# define DEAD_COLOR 0x000000
# define GRID_COLOR 0x0000FF

#include "../minilibx-linux/mlx.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_mlx
{
	void			*mlx_ptr;
	void			*win_ptr;
	int				width;
	int				height;
}					t_mlx;

typedef struct s_img
{
	void			*img_ptr;
	char			*img_addr;
	int				bpp;
	int				line_len;
	int				endian;
}					t_img;

typedef struct cell
{
    int x;
    int y;
}                   t_cell;

t_mlx	start_mlx(int width, int height);
t_img	*init_img(t_mlx *window);
int	    *get_pixel_from_img(t_img *img, int x, int y);
void	img_pix_put(t_img *img, int x, int y, int color);

#endif